

#include "graphics.h"
#include "defines.h"


Graphics::Graphics( )
: m_pGLRenderer( 0 )
{

}

Graphics::~Graphics( )
{
	Release( );
}

bool
Graphics::Initialise( )
{
	return true;
}

bool 
Graphics::InitialiseOpenGL( HWND _hWnd )
{
	bool bResult = false;
	if ( !m_pGLRenderer )
	{
		m_pGLRenderer = new OpenGLRenderer( );
	}
	m_pGLRenderer->SetWindow( _hWnd );

	bResult = m_pGLRenderer->InitialiseExtensions( );
	ErrAssert( bResult, L"EXTENSIONS FAILED!!" );

	return bResult;
}

void
Graphics::BeginScene( glm::vec4& _clearColour )
{
	m_pGLRenderer->BeginScene( _clearColour );
}

void
Graphics::EndScene( )
{
	m_pGLRenderer->EndScene( );
}

OpenGLRenderer*
Graphics::GetOpenGL( ) const
{
	return m_pGLRenderer;
}

void
Graphics::Release( )
{
	SAFEDELETE( m_pGLRenderer );
}

