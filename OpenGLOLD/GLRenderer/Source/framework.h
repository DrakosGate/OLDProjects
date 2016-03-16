#pragma once

#define WIN32_LEAN_AND_MEAN
#include "glrenderer.h"

class Engine;

class FrameWork
{
//Member functions
public:
	FrameWork( );
	~FrameWork( );

	void Initalise( );
	void Run( );

private:
	bool CreateOpenGLWindow( wchar_t* _pTitle, int _iX, int _iY, int _iWidth, int _iHeight, bool _bFullscreen, bool _bVSync );

//Member variables
private:
	Engine* m_pEngine;
	HINSTANCE m_hInstance;

	wchar_t* m_pWindowTitle;
	bool m_bIsFullscreen;

};