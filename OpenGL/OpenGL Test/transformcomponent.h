

#pragma once

#include <vector>

#include "mathlibrary.h"
#include "component.h"

class GameObject;

namespace Component
{
	class Transform : public BaseComponent
	{
	public:
		Transform( );
		~Transform( );

		virtual bool Initialise( GameObject* _pOwner );
		virtual void Process( float _fDeltaTime );
		virtual const char* GetName( ) override { return "Transform"; }

		//Parent functions
		virtual void AddChild( GameObject* _pChild );
		virtual void SetParent( GameObject* _pParent );
		
		virtual const Math::Matrix& GetMatrix() const;

		//PRS Accessor functions
		virtual void SetPosition( Math::Vector3& _rVecPosition );
		virtual Math::Vector3& GetPosition( );
		virtual void SetRotation( Math::Vector3& _rRotation );
		virtual Math::Vector3& GetRotation( );
		virtual void SetScale( Math::Vector3& _rScale );
		virtual Math::Vector3& GetScale( );

		//Vector Accessor functions
		virtual void SetForward( Math::Vector3& _rVecPosition );
		virtual Math::Vector3& GetForward( );
		virtual void SetRight( Math::Vector3& _rVecRight );
		virtual Math::Vector3& GetRight( );
		virtual void SetUp( Math::Vector3& _rVecUp );
		virtual Math::Vector3& GetUp( );

		Math::Vector3 VecConvert( const Math::Vector3& _original ) { return Math::Vector3( _original.x, -_original.y, _original.z ); }		   

	protected:
		Math::Matrix m_matWorld;

		Math::Vector3 m_vecPosition;
		Math::Vector3 m_vecScale;
		Math::Vector3 m_vecRotation;

		Math::Vector3 m_vecForward;
		Math::Vector3 m_vecRight;
		Math::Vector3 m_vecUp;

		GameObject* m_pParent;
		std::vector< GameObject* > m_vecChildren;

	};
}