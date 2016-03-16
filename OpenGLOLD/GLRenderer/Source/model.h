//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   model.h
//  Description :   Class of Model
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __MODEL_H__
#define __MODEL_H__

// Library Includes

// Local Includes
#include "gameobject.h"
#include "defines.h"

// Types

// Constants

// Prototypes
class OpenGLRenderer;
class Texture;
class Shader;

class Model : public GameObject
{
public:
	Model();
	~Model();

	virtual bool Initialise(OpenGLRenderer* _pRenderer, glm::vec3& _rPosition, float _fScale, ETextureType _eTextureType, Shader* _pShader);
	virtual void LoadSquare();
	virtual void LoadFromOBJ(char* _pcFilename);
	virtual void CreateModelData(TVertexType* _pVertices, unsigned int* _pIndices);
	virtual void RecalculateNormals(TVertexType* _pVertices);
	virtual void CleanupBuffers();

	virtual void Draw(OpenGLRenderer* _pRenderer, CCamera* _pCamera);

	virtual ETextureType GetTextureType() const;
	virtual void SetTextureType(ETextureType _eTextureType);
	virtual glm::mat4x4* GetWorldMatrix();

	ETextureType GetNormalMap() const;
	void SetNormalMap(ETextureType _eNormalMapTexture);

private:
	Model(const Model& _krInstanceToCopy);
	const Model& operator =(const Model& _krInstanceToCopy);
	
protected:
	OpenGLRenderer* m_pRenderer;
	ETextureType m_eTextureType;
	ETextureType m_eNormalMapTexture;
	Shader* m_pShader;
	int m_iVertexCount;
	int m_iIndexCount;
	unsigned int m_uiVertexArrayID;
	unsigned int m_uiVertexBufferID;
	unsigned int m_uiIndexBufferID;
	float m_fScale;
	float m_fTime;
	
};

#endif //__MODEL_H__