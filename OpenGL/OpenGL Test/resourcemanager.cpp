//  File Name   :   ResourceManager.cpp
//  Description :   Code for Class ResourceManager
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "model.h"
#include "texture.h"
#include "shader.h"

// This Include
#include "resourcemanager.h"

// Static Variables

// Static Function Prototypes

// Implementation
/*
*
* ResourceManager constructor
* @author Christopher Howlett
*
*/
ResourceManager::ResourceManager( )
: m_pRootNode( 0 )
, m_pResourceFilenames( 0 )
, m_pResourceIndexMap( 0 )
{

}
/*
*
* ResourceManager destructor
* @author Christopher Howlett
*
*/
ResourceManager::~ResourceManager( )
{
	//Delete all resources
	std::map<std::string, Model*>::iterator modelIter;
	std::map<std::string, Model*>::iterator animIter = m_mapAnimations.begin( );
	std::map<std::string, Texture*>::iterator textureIter;
	for ( modelIter = m_mapModels.begin( ); modelIter != m_mapModels.end( ); ++modelIter )
	{
		SAFEDELETE( modelIter->second );
	}
	for ( animIter = m_mapAnimations.begin( ); animIter != m_mapAnimations.end( ); ++animIter )
	{
		SAFEDELETE( animIter->second );
	}
	for ( textureIter = m_mapTextures.begin( ); textureIter != m_mapTextures.end( ); ++textureIter )
	{
		SAFEDELETE( textureIter->second );
	}

	//Clear maps
	if ( m_pResourceFilenames )
	{
		delete[] m_pResourceFilenames;
		m_pResourceFilenames = 0;
	}
	if ( m_pResourceIndexMap )
	{
		delete[] m_pResourceIndexMap;
		m_pResourceIndexMap = 0;
	}
	std::map<std::string, TPrefabDefinition*>::iterator prefabIter;
	for ( prefabIter = m_mapPrefabs.begin( ); prefabIter != m_mapPrefabs.end( ); ++prefabIter )
	{
		if ( prefabIter->second )
		{
			for ( unsigned int iChild = 0; iChild < prefabIter->second->vecChildren.size( ); ++iChild )
			{
				SAFEDELETE( prefabIter->second->vecChildren[iChild] );
			}
			for ( unsigned int iLight = 0; iLight < prefabIter->second->vecLights.size( ); ++iLight )
			{
				SAFEDELETE( prefabIter->second->vecLights[iLight] );
			}
			prefabIter->second->vecChildren.clear( );
			SAFEDELETE( prefabIter->second );
		}
	}
	m_mapPrefabs.clear( );

	//Clear node hierarchy
	if ( m_pRootNode )
	{
		m_pRootNode->Clear( );
		delete m_pRootNode;
		m_pRootNode = 0;
	}
}
/*
*
* KEY AREA: ResourceManager Initialise reads in all resource information from file
* @author Christopher Howlett
*
*/
bool
ResourceManager::Initialise( char* _pcResourceFilename )
{
	m_pRootNode = new TSceneNode( );
	//Initialise scene buffers
	m_pResourceFilenames = new std::vector<std::string>[RESOURCE_MAX];
	m_pResourceIndexMap = new std::map <std::string, int>[RESOURCE_MAX];

	if ( _pcResourceFilename )
	{
		//Open file containing resource information
		rapidxml::file<> xmlFile( _pcResourceFilename );
		rapidxml::xml_document<> xmlDoc;

		//Parse file string
		xmlDoc.parse<0>( xmlFile.data( ) );
		rapidxml::xml_node<>* pResourceNode = xmlDoc.first_node( "resources" );
		rapidxml::xml_node<>* pPrefabNode = xmlDoc.first_node( "prefabs" );

		LoadResources( pResourceNode );
		LoadPrefabDefinitions( pPrefabNode );
	}
	return true;
}
/*
*
* ResourceManager LoadResources
* @author Christopher Howlett
*
* @param _pResourceNode Node containing resource information
*
*/
void
ResourceManager::LoadResources( rapidxml::xml_node<>* _pResourceNode )
{
	//Find root nodes
	rapidxml::xml_node<>* pModels = _pResourceNode->first_node( "models" );
	rapidxml::xml_node<>* pAnimations = _pResourceNode->first_node( "animations" );
	rapidxml::xml_node<>* pTextures = _pResourceNode->first_node( "textures" );

	//Loop through textures
	unsigned int uiCurrentTextureIndex = 1;
	std::string sFilePrefix = pTextures->first_node( "fileprefix" )->value( );
	for ( rapidxml::xml_node<>* pCurrentTexture = pTextures->first_node( "texture" ); pCurrentTexture; pCurrentTexture = pCurrentTexture->next_sibling( ) )
	{
		std::string sTextureName = pCurrentTexture->first_attribute( "id" )->value( );
		std::string sTextureFilename = pCurrentTexture->first_node( )->value( );

		m_pResourceIndexMap[RESOURCE_TEXTURE][sTextureName] = m_pResourceFilenames[RESOURCE_TEXTURE].size( );
		m_pResourceFilenames[RESOURCE_TEXTURE].push_back( sFilePrefix + sTextureFilename );

		Texture* newTexture = new Texture( );
		newTexture->Initialise( ( sFilePrefix + sTextureFilename ).c_str( ), uiCurrentTextureIndex );
		++uiCurrentTextureIndex;

		newTexture->SetName( sTextureName );
		m_mapTextures[sTextureName] = newTexture;
	}
	//Loop through models
	sFilePrefix = pModels->first_node( "fileprefix" )->value( );
	for ( rapidxml::xml_node<>* pCurrentModel = pModels->first_node( "model" ); pCurrentModel; pCurrentModel = pCurrentModel->next_sibling( ) )
	{
		std::string sModelName = pCurrentModel->first_attribute( "id" )->value( );
		std::string sModelFilename = pCurrentModel->first_node( )->value( );

		m_pResourceIndexMap[RESOURCE_MODEL][sModelName] = m_pResourceFilenames[RESOURCE_MODEL].size( );
		m_pResourceFilenames[RESOURCE_MODEL].push_back( sFilePrefix + sModelFilename );

		Model* newModel = new Model();
		newModel->Initialise(1.0f);
		newModel->LoadFromOBJ( ( sFilePrefix + sModelFilename ).c_str() );
		m_mapModels[sModelName] = newModel;
	}
	//Loop through animated models
	sFilePrefix = pAnimations->first_node( "fileprefix" )->value( );
	for ( rapidxml::xml_node<>* pCurrentAnimation = pAnimations->first_node( "model" ); pCurrentAnimation; pCurrentAnimation = pCurrentAnimation->next_sibling( ) )
	{
		std::string sAnimationName = pCurrentAnimation->first_attribute( "id" )->value( );
		std::string sAnimationFilename = pCurrentAnimation->first_node( )->value( );

		m_pResourceIndexMap[RESOURCE_ANIMATION][sAnimationName] = m_pResourceFilenames[RESOURCE_ANIMATION].size( );
		m_pResourceFilenames[RESOURCE_ANIMATION].push_back( sFilePrefix + sAnimationFilename );

		//Model* newModel = new Model( );
		//newModel->Initialise( 1.0f );
		//newModel->LoadFromOBJ( ( sFilePrefix + sAnimationFilename ).c_str( ) );
		//m_mapAnimations[sAnimationName] = newModel;
	}
}
/*
*
* ResourceManager LoadPrefabDefinitions
* @author Christopher Howlett
*
* @param _pPrefabNode Node containing prefab definitions
*
*/
void
ResourceManager::LoadPrefabDefinitions( rapidxml::xml_node<>* _pPrefabNode )
{
	//Loop through models
	for ( rapidxml::xml_node<>* pCurrentPrefab = _pPrefabNode->first_node( "prefab" ); pCurrentPrefab; pCurrentPrefab = pCurrentPrefab->next_sibling( ) )
	{
		TPrefabDefinition* pNewPrefab = new TPrefabDefinition( );
		pNewPrefab->sName = pCurrentPrefab->first_attribute( "id" )->value( );
		pNewPrefab->sModel = pCurrentPrefab->first_node( "model" )->value( );
		pNewPrefab->sTexture = pCurrentPrefab->first_node( "texture" )->value( );

		pNewPrefab->vecScale[0] = ReadFromString<float>( pCurrentPrefab->first_node( "scale" )->first_attribute( "x" )->value( ) );
		pNewPrefab->vecScale[1] = ReadFromString<float>( pCurrentPrefab->first_node( "scale" )->first_attribute( "y" )->value( ) );
		pNewPrefab->vecScale[2] = ReadFromString<float>( pCurrentPrefab->first_node( "scale" )->first_attribute( "z" )->value( ) );

		pNewPrefab->bIsAnimated = false;
		pNewPrefab->bIsStatic = false;
		if ( pCurrentPrefab->first_node( "animated" ) )
		{
			pNewPrefab->bIsAnimated = strcmp( pCurrentPrefab->first_node( "animated" )->value( ), "true" ) == 0;
		}
		if ( pCurrentPrefab->first_node( "static" ) )
		{
			pNewPrefab->bIsStatic = strcmp( pCurrentPrefab->first_node( "static" )->value( ), "true" ) == 0;
		}
		pNewPrefab->sAIType = "NONE";
		if ( pCurrentPrefab->first_node( "aitype" ) )
		{
			pNewPrefab->sAIType = pCurrentPrefab->first_node( "aitype" )->value( );
		}
		//Find children
		if ( pCurrentPrefab->first_node( "child" ) )
		{
			for ( rapidxml::xml_node<>* pCurrentChild = pCurrentPrefab->first_node( "child" ); pCurrentChild; pCurrentChild = pCurrentChild->next_sibling( "child" ) )
			{
				TSceneNode* pNewEntity = new TSceneNode( );

				//Get prefab type
				pNewEntity->tEntity.sPrefabName = pCurrentChild->first_node( "type" )->value( );

				//Get Position data
				pNewEntity->tEntity.vecPosition[0] = ReadFromString<float>( pCurrentChild->first_node( "position" )->first_attribute( "x" )->value( ) );
				pNewEntity->tEntity.vecPosition[1] = ReadFromString<float>( pCurrentChild->first_node( "position" )->first_attribute( "y" )->value( ) );
				pNewEntity->tEntity.vecPosition[2] = ReadFromString<float>( pCurrentChild->first_node( "position" )->first_attribute( "z" )->value( ) );
				//Get Scale data   
				pNewEntity->tEntity.vecScale[0] = ReadFromString<float>( pCurrentChild->first_node( "scale" )->first_attribute( "x" )->value( ) );
				pNewEntity->tEntity.vecScale[1] = ReadFromString<float>( pCurrentChild->first_node( "scale" )->first_attribute( "y" )->value( ) );
				pNewEntity->tEntity.vecScale[2] = ReadFromString<float>( pCurrentChild->first_node( "scale" )->first_attribute( "z" )->value( ) );
				//Get Rotation data
				pNewEntity->tEntity.vecRotation[0] = ReadFromString<float>( pCurrentChild->first_node( "rotation" )->first_attribute( "x" )->value( ) );
				pNewEntity->tEntity.vecRotation[1] = ReadFromString<float>( pCurrentChild->first_node( "rotation" )->first_attribute( "y" )->value( ) );
				pNewEntity->tEntity.vecRotation[2] = ReadFromString<float>( pCurrentChild->first_node( "rotation" )->first_attribute( "z" )->value( ) );
				//Get Colour data  
				pNewEntity->tEntity.colour[0] = ReadFromString<float>( pCurrentChild->first_node( "colour" )->first_attribute( "r" )->value( ) );
				pNewEntity->tEntity.colour[1] = ReadFromString<float>( pCurrentChild->first_node( "colour" )->first_attribute( "g" )->value( ) );
				pNewEntity->tEntity.colour[2] = ReadFromString<float>( pCurrentChild->first_node( "colour" )->first_attribute( "b" )->value( ) );
				pNewEntity->tEntity.colour[3] = ReadFromString<float>( pCurrentChild->first_node( "colour" )->first_attribute( "a" )->value( ) );

				pNewPrefab->vecChildren.push_back( pNewEntity );
			}
		}
		//Find lights
		if ( pCurrentPrefab->first_node( "light" ) )
		{
			for ( rapidxml::xml_node<>* pCurrentLight = pCurrentPrefab->first_node( "light" ); pCurrentLight; pCurrentLight = pCurrentLight->next_sibling( "light" ) )
			{
				TLightInformation* pNewLight = new TLightInformation( );
				pNewLight->sLightType = "point";
				pNewLight->vecPosition[0] = 0.0f;
				pNewLight->vecPosition[1] = 0.0f;
				pNewLight->vecPosition[2] = 0.0f;
				pNewPrefab->vecLights.push_back( pNewLight );
			}
		}

		m_mapPrefabs[pNewPrefab->sName] = pNewPrefab;
	}
}
/*
*
* KEY AREA: ResourceManager Reads in scene hierarchy information from level XML file
* @author Christopher Howlett
*
*/
void
ResourceManager::LoadSceneFromXML( char* _pcXMLFilename )
{
	//Clear node hierarchy
	m_pRootNode->Clear( );

	rapidxml::file<> xmlFile( _pcXMLFilename );
	rapidxml::xml_document<> xmlDoc;

	//Parse file string
	xmlDoc.parse<0>( xmlFile.data( ) );
	rapidxml::xml_node<>* pRoot = xmlDoc.first_node( );

	//Loop through models
	//printf("\n  == LOADING LEVEL FROM FILE: %s ==\n", _pcXMLFilename);
	for ( rapidxml::xml_node<>* pCurrentChild = pRoot->first_node( "child" ); pCurrentChild; pCurrentChild = pCurrentChild->next_sibling( ) )
	{
		//Create Object will recursively loop through all children of this object and create those too
		AddObject( pCurrentChild, m_pRootNode );
	}
}
/*
*
* KEY AREA: ResourceManager Adds an object to the node hierarchy
* @author Christopher Howlett
*
* @param _pNode Current XML node to interpret
* @param _pParentNode Parent node structure
*
*/
void
ResourceManager::AddObject( rapidxml::xml_node<>* _pNode, TSceneNode* _pParentNode )
{
	TSceneNode* pNewEntity = new TSceneNode( );

	//Get prefab type
	pNewEntity->tEntity.sPrefabName = _pNode->first_node( "type" )->value( );

	//Get Position data
	pNewEntity->tEntity.vecPosition[0] = ReadFromString<float>( _pNode->first_node( "position" )->first_attribute( "x" )->value( ) );
	pNewEntity->tEntity.vecPosition[1] = ReadFromString<float>( _pNode->first_node( "position" )->first_attribute( "y" )->value( ) );
	pNewEntity->tEntity.vecPosition[2] = ReadFromString<float>( _pNode->first_node( "position" )->first_attribute( "z" )->value( ) );
	//Get Scale data   
	pNewEntity->tEntity.vecScale[0] = ReadFromString<float>( _pNode->first_node( "scale" )->first_attribute( "x" )->value( ) );
	pNewEntity->tEntity.vecScale[1] = ReadFromString<float>( _pNode->first_node( "scale" )->first_attribute( "y" )->value( ) );
	pNewEntity->tEntity.vecScale[2] = ReadFromString<float>( _pNode->first_node( "scale" )->first_attribute( "z" )->value( ) );
	//Get Rotation data
	pNewEntity->tEntity.vecRotation[0] = ReadFromString<float>( _pNode->first_node( "rotation" )->first_attribute( "x" )->value( ) );
	pNewEntity->tEntity.vecRotation[1] = ReadFromString<float>( _pNode->first_node( "rotation" )->first_attribute( "y" )->value( ) );
	pNewEntity->tEntity.vecRotation[2] = ReadFromString<float>( _pNode->first_node( "rotation" )->first_attribute( "z" )->value( ) );
	//Get Colour data  
	pNewEntity->tEntity.colour[0] = ReadFromString<float>( _pNode->first_node( "colour" )->first_attribute( "r" )->value( ) );
	pNewEntity->tEntity.colour[1] = ReadFromString<float>( _pNode->first_node( "colour" )->first_attribute( "g" )->value( ) );
	pNewEntity->tEntity.colour[2] = ReadFromString<float>( _pNode->first_node( "colour" )->first_attribute( "b" )->value( ) );
	pNewEntity->tEntity.colour[3] = ReadFromString<float>( _pNode->first_node( "colour" )->first_attribute( "a" )->value( ) );

	//Check if this object has children
	if ( _pNode->first_node( "child" ) )
	{
		for ( rapidxml::xml_node<>* pCurrentChild = _pNode->first_node( "child" ); pCurrentChild; pCurrentChild = pCurrentChild->next_sibling( "child" ) )
		{
			AddObject( pCurrentChild, pNewEntity );
		}
	}
	//Loop through the prefabs children
	TPrefabDefinition* pCurrentPrefabDef = GetPrefabDefinition( pNewEntity->tEntity.sPrefabName );
	if ( pCurrentPrefabDef )
	{
		for ( unsigned int iChild = 0; iChild < pCurrentPrefabDef->vecChildren.size( ); ++iChild )
		{
			TSceneNode* pNewChild = new TSceneNode( *pCurrentPrefabDef->vecChildren[iChild] );
			pNewChild->pParent = pNewEntity;
			pNewEntity->vecChildren.push_back( pNewChild );
		}
		for ( unsigned int iLight = 0; iLight < pCurrentPrefabDef->vecLights.size( ); ++iLight )
		{
			TLightInformation* pNewLight = new TLightInformation( *pCurrentPrefabDef->vecLights[iLight] );
			pNewEntity->vecLights.push_back( pNewLight );
		}
	}
	//Check if this object has any lights attached
	if ( _pNode->first_node( "light" ) )
	{
		for ( rapidxml::xml_node<>* pCurrentLight = _pNode->first_node( "light" ); pCurrentLight; pCurrentLight = pCurrentLight->next_sibling( "light" ) )
		{
			TLightInformation* pNewLight = new TLightInformation( );
			pNewLight->sLightType = "point";
			pNewLight->vecPosition[0] = 0.0f;
			pNewLight->vecPosition[1] = 0.0f;
			pNewLight->vecPosition[2] = 0.0f;
			pNewEntity->vecLights.push_back( pNewLight );
		}
	}

	//Add new object to the level
	_pParentNode->vecChildren.push_back( pNewEntity );
	pNewEntity->pParent = _pParentNode;
}
/*
*
* KEY AREA: ResourceManager Returns the root node of the hierarchy
* @author Christopher Howlett
*
* @return Returns root node of the hierarchy
*
*/
TSceneNode*
ResourceManager::GetRootNode( ) const
{
	return m_pRootNode;
}
/**
*
* CResourceManager class GetModel
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcModelName Name of model
* @return Returns Model specified
*
*/
Model*
ResourceManager::GetModel( std::string& _pcModelName ) const
{
	return ( m_mapModels.find( _pcModelName )->second );
}
/**
*
* CResourceManager class GetAnimatedModel
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcAnimatedModelName Name of model
* @return Returns Model specified
*
*/
Model*
ResourceManager::GetAnimatedModel( std::string& _pcAnimatedModelName ) const
{
	std::string sKey = _pcAnimatedModelName;
	return ( m_mapAnimations.find( sKey )->second );
}
/**
*
* CResourceManager class GetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcTextureName Name of texture
* @return Returns Texture specified
*
*/
Texture*
ResourceManager::GetTexture( std::string& _pcTextureName ) const
{
	std::string sKey = _pcTextureName;
	return ( m_mapTextures.find( sKey )->second );
}
/*
*
* ResourceManager Adds an object to the node hierarchy
* @author Christopher Howlett
*
* @param _eResourceType Type of resource requested
* @param _iIndex Resource index
* @return Returns resource filename
*
*/
std::string&
ResourceManager::GetResourceFilename( EResourceType _eResourceType, int _iIndex )
{
	return ( m_pResourceFilenames[_eResourceType][_iIndex] );
}
/*
*
* ResourceManager Adds an object to the node hierarchy
* @author Christopher Howlett
*
* @param _eResourceType Type of resource requested
* @param _sResourceName Resource name
* @return Returns resource filename
*
*/
std::string&
ResourceManager::GetResourceFilename( EResourceType _eResourceType, std::string& _sResourceName )
{
	return ( m_pResourceFilenames[_eResourceType][m_pResourceIndexMap[_eResourceType][_sResourceName]] );
}
/*
*
* ResourceManager Adds an object to the node hierarchy
* @author Christopher Howlett
*
* @param _eResourceType Type of resource requested
* @param _iIndex Resource index
*
*/
const std::string&
ResourceManager::GetResourceName( EResourceType _eResourceType, int _iIndex )
{
	std::map<std::string, int>::iterator resourceIterator;
	resourceIterator = m_pResourceIndexMap[_eResourceType].begin( );
	for ( int iIter = 0; iIter < _iIndex; ++iIter )
	{
		++resourceIterator;
	}
	return ( resourceIterator->first );
}
/*
*
* ResourceManager Adds an object to the node hierarchy
* @author Christopher Howlett
*
* @param _pNode Current XML node to interpret
* @param _pParentNode Parent node structure
*
*/
unsigned int
ResourceManager::GetResourceCount( EResourceType _eResourceType )
{
	return m_pResourceFilenames[_eResourceType].size( );
}
/*
*
* ResourceManager Gets the Prefab Definition for the specified prefab name
* @author Christopher Howlett
*
* @param _rPrefabName Name of the prefab
* @return Returns the prefab definition
*
*/
TPrefabDefinition*
ResourceManager::GetPrefabDefinition( std::string& _rPrefabName )
{
	return ( m_mapPrefabs[_rPrefabName] );
}
/*
*
* ResourceManager Gets the Prefab Definition for the specified prefab index
* @author Christopher Howlett
*
* @param _iIndex Index of the prefab
* @return Returns the prefab definition
*
*/
TPrefabDefinition*
ResourceManager::GetPrefabDefinition( int _iIndex )
{
	std::map<std::string, TPrefabDefinition*>::iterator prefabIter;
	prefabIter = m_mapPrefabs.begin( );
	for ( int iIter = 0; iIter < _iIndex; ++iIter )
	{
		++prefabIter;
	}
	return ( prefabIter->second );
}
/*
*
* ResourceManager Gets the number of prefabs defined
* @author Christopher Howlett
*
* @return Returns the number of defined prefabs
*
*/
unsigned int
ResourceManager::GetPrefabCount( ) const
{
	return m_mapPrefabs.size( );
}