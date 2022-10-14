#include "System.h"



SYSTEM::SYSTEM(){

	if ( !Init() )
	{
		LOG_ERROR("Init System Object - Failed \n");
	}
	else
	{
		LOG_INFO("Init System Object - Succssed \n");
	}
}



SYSTEM::SYSTEM(const SYSTEM& Other)
{
	*this = Other;
}



SYSTEM::~SYSTEM(){}



bool SYSTEM::Init()
{
	// Windows Init
	InitWindows();

	return true;
}



void SYSTEM::Run()
{
	// Message Structor Create and Init
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Main Loop
	while(true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!Frame())
			{
				break;
			}
		}
	}
}



void SYSTEM::Done()
{
	LOG_INFO(" End Windows \n");
	DoneWindows();
}



bool SYSTEM::Frame()
{
	return true;
}



void SYSTEM::InitWindows()
{
	// Other Pointer(->MSGHandle)
	MSGHandle = this;

	// Get Instance for this Program
	this->m_hInstance = GetModuleHandle( NULL );

	// Set Windows' Name
	this->m_WindowName = "Window_Name";
	this->m_WindowTitle = "Window_Title";

	// Set Windows' Size
	this->m_Width = 800;
	this->m_Height = 640;

	// Set Windows' fullscreen value
	this->m_Fullscreen = false;

	// Create WNDCLASSED Structure
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = NULL;
	wcex.cbWndExtra = NULL;
	wcex.hInstance = this->m_hInstance;
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
		NULL, NULL, this->m_hInstance, NULL );

	if ( !m_hWnd )
	{
		LOG_ERROR("Create Window - Failed \n");
	}
	else
	{
		LOG_INFO("Create Window - Successed \n");
	}

	// Show and Update Window
	ShowWindow( this->m_hWnd, SW_SHOW );
	UpdateWindow( this->m_hWnd );
}



void SYSTEM::DoneWindows()
{
	// Delete Windows
	DestroyWindow( this->m_hWnd );
	this->m_hWnd = nullptr;

	// Delete Instance for this Program
	UnregisterClass( this->m_WindowName, this->m_hInstance );
	this->m_hInstance = NULL;

	// Init Other Pointer
	MSGHandle = NULL;
}



LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
		case WM_KEYDOWN :
		{
			if ( wParam == VK_ESCAPE )
			{
				LOG_INFO("Pushed Exit Key \n");
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