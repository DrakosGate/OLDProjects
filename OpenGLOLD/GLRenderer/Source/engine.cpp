
#include "engine.h"
#include "defines.h"

Engine* Engine::m_sEngine = 0;

Engine::Engine( )
: m_pGraphics( 0 )
{
	
}

Engine::~Engine( )
{
	Release( );
}

Engine* 
Engine::GetInstance( )
{
	if ( !m_sEngine )
	{
		m_sEngine = new Engine( );
	}
	return m_sEngine;
}

bool
Engine::Initialise( )
{
	bool bResult = true;
	return bResult;
}

bool
Engine::InitialiseGraphics( HWND _hWnd )
{
	m_pGraphics = new Graphics( );
	return m_pGraphics->InitialiseOpenGL( _hWnd );
}

void
Engine::Run( )
{
	m_pGraphics->BeginScene( glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	//Render scene

	m_pGraphics->EndScene( );
}

Graphics*
Engine::GetGraphics( ) const
{
	return m_pGraphics;
}

void
Engine::Release( )
{
	SAFEDELETE( m_pGraphics );
	SAFEDELETE( m_sEngine );
}
