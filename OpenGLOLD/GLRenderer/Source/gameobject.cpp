//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   gameobject.cpp
//  Description :   Code for Class GameObject
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <glm\gtc\matrix_transform.hpp>

// Local Includes

// This Include
#include "gameobject.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* GameObject constructor
*
* @author Christopher Howlett
*
*/
GameObject::GameObject()
:	m_pParent(0)
,	m_bHasParent(false)
{
	m_matWorld = glm::mat4(1.0f);
	m_vecPosition *= 0.0f;
	m_vecScale *= 0.0f;
	m_vecRotation *= 0.0f;
	
	m_vecForward = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vecRight = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vecUp = glm::vec3(0.0f, 1.0f, 0.0f);

	m_vecParentOffset *= 0.0f;
}
/**
*
* GameObject destructor
*
* @author Christopher Howlett
*
*/
GameObject::~GameObject()
{
	//Do nothing for this class
}
/**
*
* GameObject Initialise
*
* @author Christopher Howlett
*
*/
bool 
GameObject::Initialise()
{
	//Do nothing for this class
	return true;
}
/**
*
* GameObject Draw
*
* @author Christopher Howlett
* @param _pRenderer OpenGL Renderer
* @param _pCamera Camera used for rendering
*
*/
void 
GameObject::Draw(OpenGLRenderer* _pRenderer, CCamera* _pCamera)
{
	//Do nothing for this class
}
/**
*
* GameObject Process
*
* @author Christopher Howlett
* @param _fDeltaTime Game Time elapsed
*
*/
void 
GameObject::Process(float _fDeltaTime)
{
	m_matWorld = glm::translate( glm::mat4( 1.0f ), m_vecPosition );

	m_matWorld = glm::rotate( m_matWorld, m_vecRotation.x, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	m_matWorld = glm::rotate( m_matWorld, m_vecRotation.y, glm::vec3( -1.0f, 0.0f, 0.0f ) );
	m_matWorld = glm::rotate( m_matWorld, m_vecRotation.z, glm::vec3( 0.0f, 1.0f, 0.0f ) );

	m_matWorld = glm::scale( m_matWorld, m_vecScale );
}
/**
*
* Processes Parent functionality on this gameobject
*
* @author Christopher Howlett
* @param Game time elapsed
*
*/
void 
GameObject::ProcessParent(float _fDeltaTime)
{
	if(m_bHasParent)
	{
		//Move object relative to parent
		m_vecPosition = m_pParent->GetPosition();
		m_vecPosition += m_vecRight * m_vecParentOffset.x;
		m_vecPosition += m_vecUp * m_vecParentOffset.y;
		m_vecPosition += m_vecForward * m_vecParentOffset.z;
	}
}
/**
*
* Sets the parent object of this gameobject - Pass NULL or 0 to remove parent object
*
* @author Christopher Howlett
* @param Parent gameobject assigned
*
*/
void 
GameObject::SetParent(GameObject* _pParent, glm::vec3* _pTargetOffset)
{
	if(_pParent)
	{
		m_bHasParent = true;
		m_pParent = _pParent;
		m_vecParentOffset = *_pTargetOffset;
	}
	else
	{
		m_bHasParent = false;
		m_pParent = 0;
		m_vecParentOffset *= 0.0f;
	}
}
/**
*
* Sets GameObject position
*
* @author Christopher Howlett
* @param position of gameobject
*
*/
void 
GameObject::SetPosition(glm::vec3& _rVecPosition)
{
	m_vecPosition = _rVecPosition;
}
/**
*
* Returns GameObject position
*
* @author Christopher Howlett
* @return Gameobject position
*
*/
glm::vec3& 
GameObject::GetPosition()
{
	return m_vecPosition;
}
/**
*
* Sets GameObject rotation
*
* @author Christopher Howlett
* @param Gameobject rotation
*
*/
void 
GameObject::SetRotation(glm::vec3& _rRotation)
{
	m_vecRotation = _rRotation;
}
/**
*
* Returns GameObject rotation
*
* @author Christopher Howlett
* @return Gameobject rotation
*
*/
glm::vec3& 
GameObject::GetRotation()
{
	return m_vecRotation;
}
/**
*
* Sets GameObject scale
*
* @author Christopher Howlett
* @param Gameobject scale
*
*/
void 
GameObject::SetScale(glm::vec3& _rScale)
{
	m_vecScale = _rScale;
}
/**
*
* Returns GameObject scale
*
* @author Christopher Howlett
* @return Gameobject scale
*
*/
glm::vec3& 
GameObject::GetScale()
{
	return m_vecScale;
}
/**
*
* Sets GameObject Forward
*
* @author Christopher Howlett
* @param Gameobject Forward
*
*/
void 
GameObject::SetForward(glm::vec3& _rForward)
{
	m_vecForward = _rForward;
	glm::normalize( m_vecForward );
}
/**
*
* Returns GameObject Forward
*
* @author Christopher Howlett
* @return Gameobject Forward
*
*/
glm::vec3& 
GameObject::GetForward()
{
	glm::normalize( m_vecForward );
	return m_vecForward;
}
/**
*
* Sets GameObject Right
*
* @author Christopher Howlett
* @param Gameobject Forward
*
*/
void 
GameObject::SetRight(glm::vec3& _rRight)
{
	m_vecRight = _rRight;
	glm::normalize( m_vecRight );
}
/**
*
* Returns GameObject Right
*
* @author Christopher Howlett
* @return Gameobject Right
*
*/
glm::vec3& 
GameObject::GetRight()
{
	glm::normalize( m_vecRight );
	return m_vecRight;
}
/**
*
* Sets GameObject Up
*
* @author Christopher Howlett
* @param Gameobject Up
*
*/
void 
GameObject::SetUp(glm::vec3& _rUp)
{
	m_vecUp = _rUp;
	glm::normalize( m_vecUp );
}
/**
*
* Returns GameObject Up
*
* @author Christopher Howlett
* @return Gameobject Up
*
*/
glm::vec3& 
GameObject::GetUp()
{
	glm::normalize( m_vecUp );
	return m_vecUp;
}