#include "framework.h"

#include "engine.h"
#include "defines.h"

LRESULT CALLBACK WndProc( HWND _hWnd, UINT _iMsg, WPARAM _wParam, LPARAM _hParam );

FrameWork::FrameWork( )
{
	m_pWindowTitle = L"OpenGLWindow";
}

FrameWork::~FrameWork( )
{
	if ( m_bIsFullscreen )
	{
		ChangeDisplaySettings( NULL, 0 );
	}
	UnregisterClass( m_pWindowTitle, m_hInstance );
	m_hInstance = NULL;
}

void
FrameWork::Initalise( )
{
	bool bResult = CreateOpenGLWindow( m_pWindowTitle, 0, 0, 1024, 720, false, true );
	ErrAssert( bResult, L"Could not create opengl window" ); 
	
	m_pEngine = Engine::GetInstance( );
	m_pEngine->Initialise( );
}

void
FrameWork::Run( )
{
	MSG msg;
	ZeroMemory( &msg, sizeof( MSG ) );

	//Game loop
	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			//Update
			m_pEngine->Run( );
		}
	}
}

bool
FrameWork::CreateOpenGLWindow( wchar_t* _pTitle, int _iX, int _iY, int _iWidth, int _iHeight, bool _bFullscreen, bool _bVSync )
{
	bool bResult = false;
	m_bIsFullscreen = _bFullscreen;
	m_pEngine = Engine::GetInstance( );

	HWND hWnd;
	WNDCLASSEX wc;
	m_hInstance = GetModuleHandle( NULL );

	//Setup window class
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_WINLOGO );
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _pTitle;
	wc.cbSize = sizeof( WNDCLASSEX );

	RegisterClassEx( &wc );
	//ErrAssert( bResult, L"Could not create windows class" );

	//Create temporary window for extension setup
	hWnd = CreateWindowEx(	WS_EX_APPWINDOW,
							_pTitle, _pTitle,
							WS_POPUP,
							0, 0,
							640, 480,
							NULL, NULL,
							m_hInstance, NULL );
	ErrAssert( hWnd, L"Could not create temporary window" );

	ShowWindow( hWnd, SW_HIDE );

	//Initialise graphics
	bResult = m_pEngine->InitialiseGraphics( hWnd );
	ErrAssert( bResult, L"Could not initialise graphics" );

	//Destroy temporary window
	DestroyWindow( hWnd );
	hWnd = NULL;

	//Set window styles
	int winStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;
	int iScreenWidth = GetSystemMetrics( SM_CXSCREEN );
	int iScreenHeight = GetSystemMetrics( SM_CYSCREEN );
	if ( _bFullscreen )
	{
		//Fullscreen settings
		DEVMODE dScreenSettings;
		memset( &dScreenSettings, 0, sizeof( DEVMODE ) );
		dScreenSettings.dmSize = sizeof( DEVMODE );
		dScreenSettings.dmPelsWidth = static_cast<unsigned long>( iScreenWidth );
		dScreenSettings.dmPelsHeight = static_cast<unsigned long>( iScreenHeight );
		dScreenSettings.dmBitsPerPel = 32;
		dScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings( &dScreenSettings, CDS_FULLSCREEN );
	}
	else
	{
		iScreenWidth = _iWidth;
		iScreenHeight = _iHeight;
	}

	//Create window
	hWnd = CreateWindowEx(	WS_EX_APPWINDOW, _pTitle, _pTitle,
							winStyle,
							_iX, _iY,
							iScreenWidth, iScreenHeight,
							NULL, NULL,
							m_hInstance, NULL );
	ErrAssert( hWnd, L"Window creation failed" );

	m_pEngine->GetGraphics( )->GetOpenGL( )->SetWindow( hWnd );
	bResult = m_pEngine->GetGraphics( )->GetOpenGL( )->InitialiseOpenGL( _bVSync );

	//Show window
	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );

	return bResult;
}

LRESULT CALLBACK WndProc( HWND _hWnd, UINT _iMsg, WPARAM _wParam, LPARAM _lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch ( _iMsg )
	{
	case WM_KEYDOWN:
	{
		if ( _wParam == VK_ESCAPE )
		{
			PostQuitMessage( 0 );
			DestroyWindow( _hWnd );
		}
		break;
	}
	case WM_KEYUP:
	{
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint( _hWnd, &ps );
		EndPaint( _hWnd, &ps );
		break;
	}
	default:
		return DefWindowProc( _hWnd, _iMsg, _wParam, _lParam );
	};

	return 0;
}