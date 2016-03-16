

#include "transformcomponent.h"

namespace Component
{
	Transform::Transform( )
	{
		SetPosition( Math::Vector3( 0.1f, 0.1f, 0.1f ) );
		SetScale( Math::Vector3( 1.0f, 1.0f, 1.0f ) ); 
		SetRotation( Math::Vector3( 0.0f, 0.0f, 0.0f ) );

		SetForward( Math::Vector3( 0.0f, 0.0f, 1.0f ) );
		SetRight( Math::Vector3( 1.0f, 0.0f, 0.0f ) );
		SetUp( Math::Vector3( 0.0f, 1.0f, 0.0f ) );
	}

	Transform::~Transform( )
	{

	}

	bool
	Transform::Initialise( GameObject* _pOwner )
	{
		return true;
	}

	void
	Transform::Process( float _fDeltaTime )
	{
		Math::Matrix matTranslate = Math::Translate( Math::Matrix( 1.0f ), VecConvert( m_vecPosition ) );

		Math::Vector3 convertedRotation = VecConvert( m_vecRotation );
		Math::Matrix matRotate = Math::Rotate( Math::Matrix( 1.0f ), convertedRotation.x, Math::Vector3( 1.0f, 0.0f, 0.0f ) );
		matRotate = Math::Rotate( matRotate, convertedRotation.y, Math::Vector3( 0.0f, 1.0f, 0.0f ) );
		matRotate = Math::Rotate( matRotate, convertedRotation.z, Math::Vector3( 0.0f, 0.0f, 1.0f ) );

		//Math::Quaternion tempRot = glm::toQuat( );
		//Math::Matrix matRotate = glm::toMat4( normalize( tempRot ) );

		Math::Matrix matScale = Math::Scale( Math::Matrix( 1.0f ), m_vecScale );
			
		m_matWorld = matTranslate * matRotate * matScale;
	}

	void
	Transform::AddChild( GameObject* _pChild )
	{
		m_vecChildren.push_back( _pChild );
	}

	void
	Transform::SetParent( GameObject* _pParent )
	{
		m_pParent = _pParent;
	}

	const Math::Matrix& 
	Transform::GetMatrix( ) const
	{
		return m_matWorld;
	}

	/**
	*
	* Sets CTransform position
	*
	* @author Christopher Howlett
	* @param position of Transform
	*
	*/
	void 
	Transform::SetPosition(Math::Vector3& _rVecPosition)
	{
		m_vecPosition = _rVecPosition;
	}
	/**
	*
	* Returns CTransform position
	*
	* @author Christopher Howlett
	* @return Transform position
	*
	*/
	Math::Vector3& 
	Transform::GetPosition()
	{
		return m_vecPosition;
	}
	/**
	*
	* Sets Transform rotation
	*
	* @author Christopher Howlett
	* @param Transform rotation
	*
	*/
	void 
	Transform::SetRotation(Math::Vector3& _rRotation)
	{
		m_vecRotation = _rRotation;
	}
	/**
	*
	* Returns Transform rotation
	*
	* @author Christopher Howlett
	* @return Transform rotation
	*
	*/
	Math::Vector3&
	Transform::GetRotation()
	{
		return m_vecRotation;
	}
	/**
	*
	* Sets Transform scale
	*
	* @author Christopher Howlett
	* @param Transform scale
	*
	*/
	void 
	Transform::SetScale(Math::Vector3& _rScale)
	{
		m_vecScale = _rScale;
	}
	/**
	*
	* Returns Transform scale
	*
	* @author Christopher Howlett
	* @return Transform scale
	*
	*/
	Math::Vector3& 
	Transform::GetScale()
	{
		return m_vecScale;
	}
	/**
	*
	* Sets Transform Forward
	*
	* @author Christopher Howlett
	* @param Transform Forward
	*
	*/
	void 
	Transform::SetForward(Math::Vector3& _rForward)
	{
		m_vecForward = _rForward;
		glm::normalize( m_vecForward );
	}
	/**
	*
	* Returns Transform Forward
	*
	* @author Christopher Howlett
	* @return Transform Forward
	*
	*/
	Math::Vector3& 
	Transform::GetForward()
	{
		glm::normalize( m_vecForward );
		return m_vecForward;
	}
	/**
	*
	* Sets Transform Right
	*
	* @author Christopher Howlett
	* @param Transform Forward
	*
	*/
	void 
	Transform::SetRight(Math::Vector3& _rRight)
	{
		m_vecRight = _rRight;
		glm::normalize( m_vecRight );
	}
	/**
	*
	* Returns Transform Right
	*
	* @author Christopher Howlett
	* @return Transform Right
	*
	*/
	Math::Vector3& 
	Transform::GetRight()
	{
		glm::normalize( m_vecRight );
		return m_vecRight;
	}
	/**
	*
	* Sets Transform Up
	*
	* @author Christopher Howlett
	* @param Transform Up
	*
	*/
	void 
	Transform::SetUp(Math::Vector3& _rUp)
	{
		m_vecUp = _rUp;
		glm::normalize( m_vecUp );
	}
	/**
	*
	* Returns Transform Up
	*
	* @author Christopher Howlett
	* @return Transform Up
	*
	*/
	Math::Vector3& 
	Transform::GetUp()
	{
		glm::normalize( m_vecUp );
		return m_vecUp;
	}

}//end namespace