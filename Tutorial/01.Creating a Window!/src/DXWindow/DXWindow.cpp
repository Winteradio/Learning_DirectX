#include "DXWindow.h"

DXWINDOW::DXWINDOW(){}

DXWINDOW::DXWINDOW( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	// Create the Window
	if ( !InitWindow( hInstance, nShowCmd, this->m_Width, this->m_Height, this->m_Fullscreen ))
	{
		LOG_ERROR("Window Initalization - Failed \n");
	}
	else {
		LOG_INFO("Window Initalization - Successed \n");
	}

	// Start the Main Loop
	MainLoop();
}

DXWINDOW::~DXWINDOW()
{
	this->m_hWnd = NULL;

	LOG_INFO(" Destory Window \n");
}

bool DXWINDOW::InitWindow( HINSTANCE hInstance, int ShowWnd, int Width, int Height, bool Fullscreen )
{
	// Make Window's Name
	this->m_WindowName = "Window_Name";
	this->m_WindowTitle = "Window_Title";

	// Create WNDCLASSEX Structure
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = NULL;
	wcex.cbWndExtra = NULL;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wcex.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW+2 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = this->m_WindowName;

	// Register Window CLass
	if ( !RegisterClassEx( &wcex ) )
	{
		LOG_ERROR("Register Class - Failed \n");
		return false;
	}
	else
	{
		LOG_INFO("Register Class - Sucessed \n");
	}

	// Create Window
	this->m_hWnd = CreateWindowEx( NULL,
		this->m_WindowName, this->m_WindowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		this->m_Width, this->m_Height,
		NULL, NULL, hInstance, NULL );

	if ( !m_hWnd )
	{
		LOG_ERROR("Create Window - Failed");
		return false;
	}
	else
	{
		LOG_INFO("Create Window - Successed");
	}

	// Show and Update Window
	ShowWindow( this->m_hWnd, ShowWnd );
	UpdateWindow( this->m_hWnd );

	return true;
}

void DXWINDOW::MainLoop()
{
	MSG msg;
	ZeroMemory( &msg, sizeof( MSG ) );

	while( true )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
			{
				break;
			}
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
	}
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
		case WM_KEYDOWN :
		{
			if ( wParam == VK_ESCAPE )
			{
				LOG_INFO("Pushed Exit Key");
				DestroyWindow( hWnd );
				return 0;
			}
		}

		case WM_DESTROY :
		{
			PostQuitMessage( 0 );
			return 0;
		}
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}