//  File Name   :   ResourceManager.h
//  Description :   Class of ResourceManager
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#ifndef __EDITORINTERPRETER_H__
#define __EDITORINTERPRETER_H__


// Library Includes
#include <rapidxml_utils.hpp>
#include <map>

// Local Includes
#include "defines.h"

// Types
enum EResourceType
{
	RESOURCE_INVALID = -1,
	RESOURCE_MODEL,
	RESOURCE_TEXTURE,
	RESOURCE_ANIMATION,
	RESOURCE_MAX
};
struct TSceneNode;
struct TPrefabInstance
{
	std::string sPrefabName;
	float vecPosition[3];
	float vecScale[3];
	float vecRotation[3];
	float colour[4];
};
struct TLightInformation
{
	TLightInformation( ){}
	TLightInformation( std::string& _sLightType, float _pVecPosition[3] )
	{
		sLightType = _sLightType;
		for ( int iPos = 0; iPos < 3; ++iPos )
		{
			vecPosition[iPos] = _pVecPosition[iPos];
		}
	}
	std::string sLightType;
	float vecPosition[3];
};
struct TPrefabDefinition
{
	TPrefabDefinition( ){}
	TPrefabDefinition( std::string& _rName,
					   std::string& _rModel,
					   std::string& _rTexture,
					   float _pVecScale[3],
					   std::string& _rAIType,
					   bool _bIsAnimated,
					   bool _bIsStatic )
	{
		sName = _rName;
		sModel = _rModel;
		sTexture = _rTexture;
		for ( int iScale = 0; iScale < 3; ++iScale )
		{
			vecScale[iScale] = _pVecScale[iScale];
		}

		sAIType = _rAIType;
		bIsAnimated = _bIsAnimated;
		bIsStatic = _bIsStatic;
	}
	std::string sName;
	std::string sModel;
	std::string sTexture;
	float vecScale[3];

	std::string sAIType;
	bool bIsAnimated;
	bool bIsStatic;

	std::vector<TSceneNode*> vecChildren;
	std::vector<TLightInformation*> vecLights;
};
struct TSceneNode
{
	TSceneNode( )
	: pParent( 0 )
	{
	}
	void Clear( )
	{
		pParent = 0;
		for ( unsigned int iChild = 0; iChild < vecChildren.size( ); ++iChild )
		{
			vecChildren[iChild]->Clear( );
			delete vecChildren[iChild];
			vecChildren[iChild] = 0;
		}
		vecChildren.clear( );
		for ( unsigned int iLight = 0; iLight < vecLights.size( ); ++iLight )
		{
			delete vecLights[iLight];
			vecLights[iLight] = 0;
		}
		vecLights.clear( );
	}
	TPrefabInstance tEntity;
	TSceneNode* pParent;
	std::vector<TSceneNode*> vecChildren;
	std::vector<TLightInformation*> vecLights;
};

// Constants

// Prototypes
class Texture;
class Model;

class ResourceManager
{
	//Member functions
public:
	ResourceManager( );
	virtual ~ResourceManager( );

	bool Initialise( char* _pcResourceFilename );
	//Load from file
	void LoadResources( rapidxml::xml_node<>* _pResourceNode );
	void LoadPrefabDefinitions( rapidxml::xml_node<>* _pPrefabNode );
	void LoadSceneFromXML( char* _pcXMLFilename );
	//Add to hierarchy
	void AddObject( rapidxml::xml_node<>* _pNode, TSceneNode* _pParentNode );
	//Retrieve from hierarchy
	TSceneNode* GetRootNode( ) const;

	//Resource accessors
	Model*  GetModel( std::string& _pcModelName ) const;
	Model* GetAnimatedModel( std::string& _pcAnimatedModelName ) const;
	Texture* GetTexture( std::string& _pcTextureName ) const;
	const int GetTextureID( std::string& _pcTextureName ) const;
	const int GetTextureID( Texture* _pTexture ) const;


	//Resource accessors
	std::string& GetResourceFilename( EResourceType _eResourceType, int _iIndex );
	std::string& GetResourceFilename( EResourceType _eResourceType, std::string& _sResourceName );
	const std::string& GetResourceName( EResourceType _eResourceType, int _iIndex );
	unsigned int GetResourceCount( EResourceType _eResourceType );
	//Prefab accessors
	TPrefabDefinition* GetPrefabDefinition( std::string& _rPrefabName );
	TPrefabDefinition* GetPrefabDefinition( int _iIndex );
	unsigned int GetPrefabCount( ) const;

	//Member variables
private:
	std::map<std::string, Model*> m_mapModels;
	std::map<std::string, Model*> m_mapAnimations;
	std::map<std::string, Texture*> m_mapTextures;

	TSceneNode* m_pRootNode;
	std::vector<std::string>* m_pResourceFilenames;
	std::map<std::string, int>* m_pResourceIndexMap;

	std::map<std::string, TPrefabDefinition*> m_mapPrefabs;

};

#endif //__EDITORINTERPRETER_H__