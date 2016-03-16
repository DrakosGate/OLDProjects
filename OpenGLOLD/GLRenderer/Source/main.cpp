
#include <GL\glew.h>

#include "framework.h"
#include "defines.h"

int main()
{
	FrameWork* pFramework = new FrameWork( );;
	pFramework->Initalise( );

	pFramework->Run( );

	SAFEDELETE( pFramework );

	return 0;
}