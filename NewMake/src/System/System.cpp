#include "System.h"


SYSTEM::SYSTEM(){

	if ( !Init() )
	{
		LOG_ERROR(" Failed - Init System Object \n ");
	}
	else
	{
		LOG_INFO(" Succssed - Init System Object \n");
	}
}


SYSTEM::SYSTEM(const SYSTEM* Other)
{
	*this = *Other;
}


SYSTEM::~SYSTEM(){}


bool SYSTEM::Init()
{
	// Windows Init
	if ( !InitWindows() )
	{
		LOG_ERROR(" Failed - Init Windows \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Init Windows \n ");
	}

	// Create DXEngine Objects
	m_DXEngine = new DXENGINE;
	if ( !m_DXEngine )
	{
		LOG_ERROR(" Failed - Create DXENGINE \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create DXENGIN \n ");
	}

	return m_DXEngine->Init( m_Width, m_Height, m_hWnd );
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


void SYSTEM::Release()
{
	// Release DXEngine Objects
	if ( m_DXEngine )
	{
		m_DXEngine->Release();
		delete m_DXEngine;
		m_DXEngine = nullptr;

		LOG_INFO(" Delete - DXENGINE \n");
	}

	ReleaseWindows();
	LOG_INFO(" End - Windows \n");
}


bool SYSTEM::Frame()
{
	return m_DXEngine->Frame();
}


bool SYSTEM::InitWindows()
{
	// Other Pointer(->MSGHandle)
	MSGHandle = this;

	// Get Instance for this Program
	m_hInstance = GetModuleHandle( NULL );

	// Set Windows' Name
	m_WindowName = "Window_Name";
	m_WindowTitle = "Window_Title";

	// Set Windows' Size
	m_Width = 800;
	m_Height = 640;

	// Set Windows' fullscreen value
	m_FullScreen = false;

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
		LOG_ERROR(" Failed - Register Wnd Class \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Register Wnd Class \n ");
	}

	// Create Window
	m_hWnd = CreateWindowEx( NULL,
		m_WindowName, m_WindowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		m_Width, m_Height,
		NULL, NULL, m_hInstance, NULL );

	if ( !m_hWnd )
	{
		LOG_ERROR(" Failed - Create Window \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Window \n ");
	}

	// Show and Update Window
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	return true;
}


void SYSTEM::ReleaseWindows()
{
	// Delete Windows
	DestroyWindow( m_hWnd );
	m_hWnd = nullptr;

	// Delete Instance for this Program
	UnregisterClass( m_WindowName, m_hInstance );
	m_hInstance = NULL;

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