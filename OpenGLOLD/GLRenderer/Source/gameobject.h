//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   gameobject.h
//  Description :   Class of GameObject
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

// Library Includes
#include <glm\glm.hpp>

// Local Includes

// Types

// Constants

// Prototypes
class OpenGLRenderer;
class CCamera;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	
	virtual bool Initialise();
	virtual void Draw(OpenGLRenderer* _pRenderer, CCamera* _pCamera);
	virtual void Process(float _fDeltaTime);

	//Parent functions
	virtual void ProcessParent(float _fDeltaTime);
	virtual void SetParent(GameObject* _pParent, glm::vec3* _pTargetOffset);	

	//PRS Accessor functions
	virtual void SetPosition(glm::vec3& _rVecPosition);
	virtual glm::vec3& GetPosition();
	virtual void SetRotation(glm::vec3& _rRotation);
	virtual glm::vec3& GetRotation();
	virtual void SetScale(glm::vec3& _rScale);
	virtual glm::vec3& GetScale();

	//Vector Accessor functions
	virtual void SetForward(glm::vec3& _rVecPosition);
	virtual glm::vec3& GetForward();
	virtual void SetRight(glm::vec3& _rVecRight);
	virtual glm::vec3& GetRight();
	virtual void SetUp(glm::vec3& _rVecUp);
	virtual glm::vec3& GetUp();
	
private:
	GameObject(const GameObject& _kr);
	GameObject& operator= (const GameObject& _rhs);
	
protected:
	glm::mat4x4 m_matWorld;

	glm::vec3 m_vecPosition;
	glm::vec3 m_vecScale;
	glm::vec3 m_vecRotation;

	glm::vec3 m_vecForward;
	glm::vec3 m_vecRight;
	glm::vec3 m_vecUp;

	bool m_bHasParent;
	GameObject* m_pParent;
	glm::vec3 m_vecParentOffset;
};

#endif //__GAMEOBJECT_H__