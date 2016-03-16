
#pragma once

#include <cassert>
#include <glm\glm.hpp>

//Useful defines
#define SAFEDELETE(_object) { if(_object){ delete _object;_object = 0; } }
#define SAFEDELETEARRAY(_array){ if(_array){ delete[] _array; _array = 0; } }
#define ReleaseCOM(_COM) { if(_COM){ _COM->Release();_COM = 0; } }
#define Error(pwcErrorMessage) MessageBox(NULL, pwcErrorMessage, L"ERROR", MB_OK)
#define ErrAssert(expression, pwcErrorMessage) if( !expression ){ MessageBox(NULL, pwcErrorMessage, L"ERROR", MB_OK);  assert( expression ); }

//Global variables

//Vertex structures
struct TVertexType
{
	glm::vec3 vecPos;
	glm::vec2 texCoord;
	glm::vec3 vecNormal;
	glm::vec3 vecTangent;
};
//Texture types
enum ETextureType
{
	TEXTURE_INVALID = -1,
	TEXTURE_STONE,
	TEXTURE_STONENORMAL,
	TEXTURE_HUMAN,
	TEXTURE_TREE,
	TEXTURE_GRASS,
	TEXTURE_SQUARES,
	TEXTURE_FLOORTILE,
	TEXTURE_FLOORNORMAL,
	TEXTURE_SHIP,
	TEXTURE_SHIPNORMAL,
	TEXTURE_MAX
};
enum EModelType
{
	MODEL_INVALID = -1,
	MODEL_HELICHASSIS,
	MODEL_HELIROTOR,
	MODEL_SHIP,
	MODEL_LOOP,
	MODEL_CUBE,
	MODEL_MAX
};