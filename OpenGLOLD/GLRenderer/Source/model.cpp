//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   Model.cpp
//  Description :   Code for Class Model
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

// Local Includes
#include "glrenderer.h"
#include "texture.h"
#include "shader.h"

// This Include
#include "model.h"

// Static Variables

// Static Function Prototypes

// Implementation
using namespace std;
/**
*
* Model constructor
*
* @author Christopher Howlett
*
*/
Model::Model()
:	m_iVertexCount(0)
,	m_iIndexCount(0)
,	m_uiVertexArrayID(0)
,	m_uiVertexBufferID(0)
,	m_uiIndexBufferID(0)
,	m_pRenderer(0)
,	m_eTextureType(TEXTURE_INVALID)
,	m_eNormalMapTexture(TEXTURE_INVALID)
,	m_pShader(0)
,	m_fTime(0.0f)
,	m_fScale(0.0f)
{
	m_vecPosition *= 0.0f;
	m_vecRotation *= 0.0f;

	m_vecScale.x = 1.0f;
	m_vecScale.y = 1.0f;
	m_vecScale.z = 1.0f;

	m_matWorld = glm::mat4( 1.0f );
}
/**
*
* Model Destructor
*
* @author Christopher Howlett
*
*/
Model::~Model()
{
	//Release vertex and index buffers
	CleanupBuffers();
}
/**
*
* Model Initialisation
*
* @author Christopher Howlett
*
*/
bool 
Model::Initialise(OpenGLRenderer* _pRenderer, glm::vec3& _rPosition, float _fScale, ETextureType _eTextureType, Shader* _pShader)
{
	m_pRenderer = _pRenderer;
	m_vecPosition = _rPosition;
	m_fScale = _fScale;
	m_eTextureType = _eTextureType;
	m_pShader = _pShader;

	return true;
}
/**
*
* Model Load Square model data
*
* @author Christopher Howlett
*
*/
void 
Model::LoadSquare()
{
	float fHalfScale = m_fScale * 0.5f;
	
	TVertexType* pVertices;
	unsigned int* pIndices;
	m_iVertexCount = 4;
	m_iIndexCount = 6;

	pVertices = new TVertexType[m_iVertexCount];
	pIndices = new unsigned int[m_iIndexCount];

	//Initialise vertices
	//Bottom left
	pVertices[0].vecPos = glm::vec3(-fHalfScale, fHalfScale, 0.0f) + m_vecPosition;
	pVertices[0].texCoord = glm::vec2(0.0f, 1.0f);
	pVertices[0].vecNormal = glm::vec3(0.0f, 0.0f, 1.0f);
	pVertices[0].vecTangent = glm::vec3(1.0f, 0.0f, 0.0f);
	//Top left
	pVertices[1].vecPos = glm::vec3(-fHalfScale, -fHalfScale, 0.0f) + m_vecPosition;
	pVertices[1].texCoord = glm::vec2(0.0f, 0.0f);
	pVertices[1].vecNormal = glm::vec3(0.0f, 0.0f, 1.0f);
	pVertices[1].vecTangent = glm::vec3(1.0f, 0.0f, 0.0f);
	//Top right
	pVertices[2].vecPos = glm::vec3(fHalfScale, -fHalfScale, 0.0f) + m_vecPosition;
	pVertices[2].texCoord = glm::vec2(1.0f, 0.0f);
	pVertices[2].vecNormal = glm::vec3(0.0f, 0.0f, 1.0f);
	pVertices[2].vecTangent = glm::vec3(1.0f, 0.0f, 0.0f);
	//Bottom right
	pVertices[3].vecPos = glm::vec3(fHalfScale, fHalfScale, 0.0f) + m_vecPosition;
	pVertices[3].texCoord = glm::vec2(1.0f, 1.0f);
	pVertices[3].vecNormal = glm::vec3(0.0f, 0.0f, 1.0f);
	pVertices[3].vecTangent = glm::vec3(1.0f, 0.0f, 0.0f);
	
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 2;
	pIndices[4] = 3;
	pIndices[5] = 0;

	CreateModelData(pVertices, pIndices);

	//Delete vertex and index data
	delete[] pVertices;
	delete[] pIndices;
	pVertices = 0;
	pIndices = 0;
}
/**
*
* Model Load model data from an OBJ file
*
* @author Christopher Howlett
*
*/
void 
Model::LoadFromOBJ(char* _pcFilename)
{
	float fHalfScale = m_fScale * 0.5f;
	printf("Loading model %s\n", _pcFilename);
	
	TVertexType* pVertices;
	unsigned int* pIndices;
	ifstream modelFile;
	char cTextBuffer[256];
	
	//Temporary data
	glm::vec3* pPositions;
	glm::vec2* pTexCoords;
	glm::vec3* pNormals;
	glm::vec3* pFaces;
	int iNumPositions = 0;
	int iNumFaces = 0;
	int iNumNormals = 0;
	int iNumTexCoords = 0;
	
	//Open model file
	modelFile.open(_pcFilename);
	if(modelFile.is_open())
	{
		//Count vertices
		while(!modelFile.eof())
		{
			modelFile.getline(cTextBuffer, 256);
			if(cTextBuffer[0] == 'v')
			{
				//Found a vertex
				if(cTextBuffer[1] == ' ')
				{
					++iNumPositions;
				}
				//Texture coordinate data
				else if(cTextBuffer[1] == 't')
				{
					++iNumTexCoords;
				}
				//Normal data
				else if(cTextBuffer[1] == 'n')
				{
					++iNumNormals;
				}
			}
			//Face data
			else if(cTextBuffer[0] == 'f')
			{
				iNumFaces += 3;
			}
	
		}
		modelFile.close();
		modelFile.open(_pcFilename);
		//Create vertex and index array
		m_iVertexCount = iNumFaces;
		m_iIndexCount = iNumFaces;
		pVertices = new TVertexType[m_iVertexCount];
		pIndices = new unsigned int[m_iIndexCount];
	
		//Create temporary containers
		pPositions = new glm::vec3[iNumPositions];
		pTexCoords = new glm::vec2[iNumTexCoords];
		pNormals = new glm::vec3[iNumNormals];
		pFaces = new glm::vec3[iNumFaces];
	
		//Counters
		int iCurrentPosition = 0;
		int iCurrentTex = 0;
		int iCurrentNormal = 0;
		int iCurrentFace = 0;
		char cStartLetter;
		char cInput;
		while(!modelFile.eof())
		{
			modelFile.get(cStartLetter);
			//Starts with 'v'
			if(cStartLetter == 'v')
			{
				modelFile.get(cStartLetter);
				//Position data
				if(cStartLetter == ' ')
				{
					modelFile >> pPositions[iCurrentPosition].x >> pPositions[iCurrentPosition].y >> pPositions[iCurrentPosition].z;
					++iCurrentPosition;
				}
				//Texture coordinate data
				if(cStartLetter == 't')
				{
					modelFile >> pTexCoords[iCurrentTex].x >> pTexCoords[iCurrentTex].y;
					++iCurrentTex;
				}
				//Normal data
				if(cStartLetter == 'n')
				{
					modelFile >> pNormals[iCurrentNormal].x >> pNormals[iCurrentNormal].y >> pNormals[iCurrentNormal].z;
					++iCurrentNormal;
				}
			}
			//Faces
			else if(cStartLetter == 'f')
			{
				modelFile.get(cStartLetter);
				if(cStartLetter == ' ')
				{
					//Account for backwards winding
					if(iCurrentFace < iNumFaces)
					{
						modelFile	>> pFaces[iCurrentFace + 2].x >> cInput >> pFaces[iCurrentFace + 2].y >> cInput >> pFaces[iCurrentFace + 2].z
									>> pFaces[iCurrentFace + 1].x >> cInput >> pFaces[iCurrentFace + 1].y >> cInput >> pFaces[iCurrentFace + 1].z
									>> pFaces[iCurrentFace].x >> cInput >> pFaces[iCurrentFace].y >> cInput >> pFaces[iCurrentFace].z;
						iCurrentFace += 3;
					}
				}
			}
		}	
		modelFile.close();
		//Create vertices based on this index order
		for(int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
		{
			pIndices[iIndex] = static_cast<int>(pFaces[iIndex].x) - 1;
			pVertices[iIndex].vecPos	= pPositions[pIndices[iIndex]] * m_fScale;
			pVertices[iIndex].texCoord	= pTexCoords[static_cast<int>(pFaces[iIndex].y) - 1];
			pVertices[iIndex].vecNormal = pNormals[static_cast<int>(pFaces[iIndex].z) - 1];
			glm::normalize( pVertices[iIndex].vecNormal );
			pVertices[iIndex].vecTangent = glm::cross(pVertices[iIndex].vecNormal, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		//Draw vertices in ascending order
		for(int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
		{
			pIndices[iIndex] = iIndex;
		}
	
		//Delete temporary data
		delete[] pTexCoords;
		delete[] pNormals;
		delete[] pPositions;
		delete[] pFaces;
		pFaces = 0;
		pPositions = 0;
		pTexCoords = 0;
		pNormals = 0;
		
		CreateModelData(pVertices, pIndices);
	}
	else
	{
		Error(L"Could not open model file");
	}	
	
	//Delete vertex and index data
	delete[] pVertices;
	delete[] pIndices;
	pVertices = 0;
	pIndices = 0;
}
/**
*
* Model Load model data into Vertex and Index buffers
*
* @author Christopher Howlett
*
*/
void 
Model::CreateModelData(TVertexType* _pVertices, unsigned int* _pIndices)
{
	//Bind these vertices and indices to index and vertex buffers
	glGenVertexArrays(1, &m_uiVertexArrayID);
	glBindVertexArray(m_uiVertexArrayID);
	glGenBuffers(1, &m_uiVertexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_iVertexCount * sizeof(TVertexType), _pVertices, GL_STATIC_DRAW);

	//Enable vertex array attributes 
	glEnableVertexAttribArray(0); //POSITION
	glEnableVertexAttribArray(1); //TEXTURE
	glEnableVertexAttribArray(2); //NORMAL
	glEnableVertexAttribArray(3); //TANGENT

	//POSITION
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(TVertexType), 0);

	//TEXTURE
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(TVertexType),  (unsigned char*)(NULL + (3 * sizeof(float))));

	//NORMAL
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(TVertexType),  (unsigned char*)(NULL + (5 * sizeof(float))));
	
	//TANGENT
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(TVertexType),  (unsigned char*)(NULL + (8 * sizeof(float))));

	//Index buffer
	glGenBuffers(1, &m_uiIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iIndexCount * sizeof(unsigned int), _pIndices, GL_STATIC_DRAW);
}
/**
*
* Model Recalculates vertex normal data
*
* @author Christopher Howlett
*
*/
void 
Model::RecalculateNormals(TVertexType* _pVertices)
{
	//Do nothing for this model - inherited classes perform different calculations
}
/**
*
* Model Cleanup vertex and index buffers
*
* @author Christopher Howlett
*
*/
void 
Model::CleanupBuffers()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Release vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_uiVertexBufferID);

	//Release Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_uiIndexBufferID);

	//Release vertex array
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_uiVertexArrayID);
}
/**
*
* Model Draw
*
* @author Christopher Howlett
* @param _pCamera Camera used to draw this model
*
*/
void 
Model::Draw(OpenGLRenderer* _pRenderer, CCamera* _pCamera)
{
	glBindVertexArray(m_uiVertexArrayID);

	glDrawElements(GL_TRIANGLES, m_iIndexCount, GL_UNSIGNED_INT, 0);
}
/**
*
* Model GetTextureType
*
* @author Christopher Howlett
* @return Returns Model texture type
*
*/
ETextureType 
Model::GetTextureType() const
{
	return m_eTextureType;
}
/**
*
* Model SetTextureType
*
* @author Christopher Howlett
* @param _eTextureType Sets Model texture type
*
*/
void  
Model::SetTextureType(ETextureType _eTextureType)
{
	m_eTextureType = _eTextureType;
}
/**
*
* Model GetWorldMatrix
*
* @author Christopher Howlett
* @return Returns Model WorldMatrix
*
*/
glm::mat4x4* 
Model::GetWorldMatrix()
{
	return &m_matWorld;
}
/**
*
* Model GetNormalMap
*
* @author Christopher Howlett
* @return Returns Model Normal Map Texture
*
*/
ETextureType 
Model::GetNormalMap() const
{
	return m_eNormalMapTexture;
}
/**
*
* Model SetNormalMap
*
* @author Christopher Howlett
* @param _eNormalMapTexture Sets Normal Map
*
*/
void 
Model::SetNormalMap(ETextureType _eNormalMapTexture)
{
	m_eNormalMapTexture = _eNormalMapTexture;
}