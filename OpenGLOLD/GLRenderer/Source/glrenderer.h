

#pragma once

#include <GL\glew.h>
#include <GL\wglew.h>
#include <glm\glm.hpp>

//DEFINES
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092

//Typedefines - used for OpenGL 4.0
typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC _hdc, HGLRC _hShared, const int* _pAttribList);

class OpenGLRenderer
{
//Member functions
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	//Initialisation
	bool InitialiseExtensions();
	bool InitialiseOpenGL( bool _bVSync );

	//Rendering
	void BeginScene( glm::vec4& _clearColour );
	void EndScene();

	//
	HWND GetWindow() const;
	void SetWindow( HWND _hWnd );

	//Get GPU Info
	void GetVideoCardInfo( char* _pInfo );

	void Release();

private:
	bool LoadExtensions();

//Member variables
private:
	HDC m_deviceContext;
	HGLRC m_renderingContext;
	HWND m_hWnd;
	PFNWGLCREATECONTEXTATTRIBSARBPROC m_wglContextAttributes;

	char m_videocardInfo[128];

};