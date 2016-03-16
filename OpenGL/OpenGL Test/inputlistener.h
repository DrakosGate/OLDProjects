

#pragma once

#include <windows.h>
#include "mathlibrary.h"

namespace Listeners
{
	
	class InputListener
	{
	public:
		InputListener( ){}
		~InputListener( ){}

		virtual void OnKeyDown( const unsigned int _iKeyID, const float _fDeltaTime ) {}
		virtual void OnKeyUp( const unsigned int _iKeyID, const float _fDeltaTime ) {}
		virtual void OnMouseButtonDown( const unsigned int _iKeyID, const float _fDeltaTime ) {}
		virtual void OnMouseButtonUp( const unsigned int _iKeyID, const float _fDeltaTime ) {}
		virtual void OnMouseMove( Math::Vector2& _offset, const float _fDeltaTime ) {}
	};

}