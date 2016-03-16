
#include <stdio.h>
#include "glrenderer.h"
#include "defines.h"


OpenGLRenderer::OpenGLRenderer( )
: m_deviceContext( 0 )
, m_renderingContext( 0 )
{

}

OpenGLRenderer::~OpenGLRenderer( )
{
	Release();
}

bool
OpenGLRenderer::InitialiseExtensions( )
{
	printf( "- Initialising OpenGL Extensions\n" );
	bool bResult = false;

	int iError = 0;
	HDC deviceContext;
	PIXELFORMATDESCRIPTOR pixelFormat;
	HGLRC renderContext;

	//Get Device context
	deviceContext = GetDC( m_hWnd );
	ErrAssert( deviceContext, L"Device context failed" );
	
	//Set default format
	iError = SetPixelFormat( deviceContext, 1, &pixelFormat );
	ErrAssert( iError == 1, L"Invalid pixel format" );
	
	//Create render context
	renderContext = wglCreateContext( deviceContext );
	ErrAssert( renderContext, L"Could not create render context" );
	
	iError = wglMakeCurrent( deviceContext, renderContext );
	ErrAssert( iError == 1, L"Could not make current context" );

	//Load OpenGL Extensions
	bResult = LoadExtensions();
	ErrAssert( bResult, L"Could not load extensions" );

	//Release
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( renderContext );
	ReleaseDC( m_hWnd, deviceContext );

	return bResult;
}

bool
OpenGLRenderer::InitialiseOpenGL( bool _bVSync )
{
	bool bResult = false;
	int iResult = 0;
	printf( "Initialising OpenGL\n" );

	PIXELFORMATDESCRIPTOR pixelFormatDesc;
	int pixelFormat;
	int attributeList[5];
	char* pVendorString;
	char* pRendererString;

	m_deviceContext = GetDC( m_hWnd );
	ErrAssert( m_deviceContext, L"Could not create device context" );

	pixelFormat = ChoosePixelFormat( m_deviceContext, &pixelFormatDesc );
	ErrAssert( pixelFormat, L"Could not get pixel format" );
	bResult = SetPixelFormat( m_deviceContext, pixelFormat, &pixelFormatDesc );
	ErrAssert( bResult, L"Could not set pixel format" );
	
	//Temp 2.1 context
	HGLRC tempContext = wglCreateContext( m_deviceContext );
	wglMakeCurrent( m_deviceContext, tempContext ); //Temp 2.1 context

	printf( "- Initialising GLEW\n" );
	GLenum result = glewInit();
	ErrAssert( result == GLEW_OK, L"Could not initialise GLEW" );

	//Set version to 4.0
	attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;
	attributeList[4] = 0;

	//Create opengl 4.0 rendering context
	m_renderingContext = wglCreateContextAttribsARB( m_deviceContext, 0, attributeList );
	if ( m_renderingContext )
	{
		printf( "== OpenGL 4.0 is supported ==\n" );
		//Replace current with 4.0
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( tempContext );
		bResult = wglMakeCurrent( m_deviceContext, m_renderingContext );
		ErrAssert( bResult, L"Could not set 4.0 context" );
	}

	pVendorString = (char*)glGetString( GL_VENDOR );
	pRendererString = (char*)glGetString( GL_RENDERER );
	strcpy_s( m_videocardInfo, pVendorString );
	strcat_s( m_videocardInfo, "-" );
	strcat_s( m_videocardInfo, pRendererString );

	//Vsync
	bResult = wglSwapIntervalEXT( _bVSync );
	ErrAssert( bResult, L"Could not set VSync varaible" );

	int glVersion[2] = { -1, -1 };
	glGetIntegerv( GL_MAJOR_VERSION, &glVersion[0] );
	glGetIntegerv( GL_MINOR_VERSION, &glVersion[1] );
	printf( "OpenGL Initialise Success: [%i.%i]\n", glVersion[0], glVersion[1] );
	printf( "GPU: %s\n", m_videocardInfo );

	return bResult;
}

void
OpenGLRenderer::BeginScene( glm::vec4& _clearColour )
{
	glClearColor( _clearColour[0], _clearColour[1], _clearColour[2], _clearColour[3] );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void
OpenGLRenderer::EndScene( )
{
	glFlush();
	SwapBuffers( m_deviceContext );
	glFinish();
}

HWND
OpenGLRenderer::GetWindow( ) const
{
	return m_hWnd;
}

void
OpenGLRenderer::SetWindow( HWND _hWnd )
{
	m_hWnd = _hWnd;
}

void
OpenGLRenderer::GetVideoCardInfo( char* _pInfo )
{
	
}

void
OpenGLRenderer::Release()
{
	if ( m_renderingContext )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_renderingContext );
		m_renderingContext = 0;
	}
	if ( m_deviceContext )
	{
		ReleaseDC( m_hWnd, m_deviceContext );
		m_deviceContext = 0;
	}
}

bool 
OpenGLRenderer::LoadExtensions()
{
	printf( "- Loading OpenGL Extensions\n" );
	bool bResult = true;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress( "wglCreateContextAttribsARB" );
	ErrAssert( wglCreateContextAttribsARB, L"Could not load extensions" );

	return bResult;
}