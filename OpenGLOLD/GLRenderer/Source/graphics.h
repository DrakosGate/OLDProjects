
#pragma once

#include "glrenderer.h"

class Graphics
{
//Member functions
public:
	Graphics( );
	~Graphics( );

	//Initialisation
	bool Initialise( );
	bool InitialiseOpenGL( HWND _hWnd );

	//Rendering
	void BeginScene( glm::vec4& _clearColour );
	void EndScene( );

	OpenGLRenderer* GetOpenGL( ) const;

	void Release( );

//Member variables
private:
	OpenGLRenderer* m_pGLRenderer;

};