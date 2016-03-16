//
//  File Name   :   camera.h
//  Description :   Declaration of camera class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

// Library Includes

// Local Includes
#include "defines.h"
#include "gameobject.h"

// Types
enum ECameraType
{
	CAMERA_INVALID = -1,
	CAMERA_ORTHOGONAL,
	CAMERA_PERSPECTIVE,
	CAMERA_MAX
};

// Constants

// Prototypes

class Camera : public GameObject
{
// Member functions
public:
	Camera();
	virtual ~Camera();
	
	virtual bool Initialise(float _fFOV,
							float _fAspectRatio,
							float _fNearClip,
							float _fFarClip,
							float _fMovementSpeed,
							float _fRotationSpeed,
							int _iScreenWidth, 
							int _iScreenHeight,
							ECameraType _eCameraType );
	virtual void Process( const float _fDeltaTick );
	void ProcessInput(const float _fDeltaTick);
	
	void SetInput(TInputStruct* _pInput);
	void SetClipPlanes(float _fNear, float _fFar);
	float GetNearPlane() const;
	float GetFarPlane() const;
	
	void SetFOV(float _fFOV);
	void SetView3D( Math::Vector3& _rVecRight, Math::Vector3& _rVecUp, Math::Vector3& _rVecDir, Math::Vector3& _rVecPos);
	void SetViewLookAt(Math::Vector3& _rVecPos, Math::Vector3& _rVecLook, Math::Vector3& _rVecUp);
	void SetViewMatrix(Math::Matrix& _rMatView);
	void SetProjMatrix(Math::Matrix& _rMatProj);
	
	Math::Matrix& GetViewMatrix();
	Math::Matrix& GetProjectionMatrix();
	Math::Matrix& GetWorldMatrix();
	
	void Calculate3DMouseCoordinates(Math::Vector2& _rVecMouse, TRay& _rRay);
	Math::Vector3 VecConvert( const Math::Vector3& _original ) { return Math::Vector3( _original.x, -_original.y, _original.z ); }

protected:
	Camera(const Camera& _kr);
	Camera& operator= (const Camera& _rhs);
// Member variables
protected:
	ECameraType m_eCameraType;
	TInputStruct* m_pInput;

	Math::Vector3 m_vecVelocity;
	float m_fMovementSpeed;
	float m_fRotationSpeed;
	float m_fSpeedBoost;
	
	Math::Matrix m_matView;
	Math::Matrix m_matProj;
	Math::Matrix m_matWorld;

	float m_fFOV;
	float m_fNearClip;
	float m_fFarClip;
	float m_fAspectRatio;

	int m_iScreenWidth;
	int m_iScreenHeight;
};

#endif //CAMERA_H__
