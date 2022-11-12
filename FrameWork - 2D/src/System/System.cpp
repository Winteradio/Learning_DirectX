#include "System.h"


SYSTEM::SYSTEM()
{
	InitPointer();

	if ( !Init() )
	{
		LOG_ERROR(" Failed - Init System Object \n ");
		system("pause");
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
	if ( !InitWindows() ) { return false; }

	// SUBCPU Init
	if ( !InitSUBCPU() ) { return false; }
	// SUBFPS Init
	if ( !InitSUBFPS() ) { return false; }
	// SUBINPUT Init
	if ( !InitSUBINPUT() ) { return false; }
	// SUBTIMER Init
	if ( !InitSUBTIMER() ) { return false; }

	// DXENGINE Init
	if ( !InitDXENGINE() ) { return false; }

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
				LOG_INFO(" Error during Run \n");
				break;
			}
		}
	}
}


void SYSTEM::Release()
{
	// Delete Windows
	DestroyWindow( m_hWnd );

	// Delete Instance for this Program
	UnregisterClass( m_WindowName, m_hInstance );

	m_SUBCPU->Release();
	delete m_SUBCPU;

	delete m_SUBFPS;
	delete m_SUBINPUT;
	delete m_SUBTIMER;

	m_DXENGINE->Release();
	delete m_DXENGINE;

	InitPointer();

	LOG_INFO(" End - Program \n");
}


bool SYSTEM::Frame()
{
	// Update Sub System
	m_SUBCPU->Frame();
	m_SUBFPS->Frame();
	m_SUBTIMER->Frame();
	m_SUBINPUT->Frame();
	int CPU = m_SUBCPU->GetCPUPercent();
	double Time = m_SUBTIMER->GetTime();
	MOUSEINFO* Mouse = m_SUBINPUT->GetMouse();
	bool rasterState = m_SUBINPUT->GetRasterizerState();
	bool insertState = m_SUBINPUT->GetInsertState();

	if ( Time > prevTime )
	{
		if ( !m_DXENGINE->Frame( m_SUBFPS->GetFPS(), CPU, Time, prevTime, Mouse, rasterState, insertState ) )
		{
			LOG_ERROR(" Failed - Frame DXENGINE \n ");
			return false;
		}
	}

	prevTime = Time;

	return true;
}


bool SYSTEM::InitWindows()
{
	// Other Pointer(->MSGHandle)
	//MSGHandle = this;

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

	RECT clientSize = { 0, 0, m_Width, m_Height };
	AdjustWindowRect( &clientSize, WS_OVERLAPPEDWINDOW, FALSE );

	// Create Window
	m_hWnd = CreateWindowEx( NULL,
		m_WindowName, m_WindowTitle,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		clientSize.right - clientSize.left,
		clientSize.bottom - clientSize.top,
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

	LOG_INFO(" Successed - Init Windows \n ");

	return true;
}


bool SYSTEM::InitSUBINPUT()
{
	// Create SUBINPUT Objects
	g_SUBINPUT = new SUBINPUT;
	if ( !g_SUBINPUT )
	{
		LOG_ERROR(" Failed - Create SUBINPUT \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create SUBINPUT \n ");
	}

	// Init INPUT Objects
	if ( !g_SUBINPUT->Init( m_Width, m_Height ) )
	{
		LOG_ERROR(" Failed - Init SUBINPUT \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init SUBINPUT \n ");
	}

	m_SUBINPUT = g_SUBINPUT;

	return true;
}


bool SYSTEM::InitSUBCPU()
{
	// Create SUBCPU Object
	m_SUBCPU = new SUBCPU;
	if ( !m_SUBCPU )
	{
		LOG_ERROR(" Failed - Create SUBCPU \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create SUBCPU \n ");
	}

	// Init SUBCPU Object
	if ( !m_SUBCPU->Init() )
	{
		LOG_ERROR(" Failed - Init SUBCPU \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init SUBCPU \n ");
	}
	return true;
}


bool SYSTEM::InitSUBFPS()
{
	// Create SUBFPS Object
	m_SUBFPS = new SUBFPS;
	if ( !m_SUBFPS )
	{
		LOG_ERROR(" Failed - Create SUBFPS \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create SUBFPS \n ");
	}

	// Init SUBFPS Object
	if ( !m_SUBFPS->Init() )
	{
		LOG_ERROR(" Failed - Init SUBFPS \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init SUBFPS \n ");
	}

	return true;
}


bool SYSTEM::InitSUBTIMER()
{
	// Create SUBTIMER Object
	m_SUBTIMER = new SUBTIMER;
	if ( !m_SUBTIMER )
	{
		LOG_ERROR(" Failed - Create SUBTIMER \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create SUBTIMER \n ");
	}

	// Init SUBTIMER Object
	if ( !m_SUBTIMER->Init() )
	{
		LOG_ERROR(" Failed - Init SUBTIMER \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init SUBTIMER \n ");
	}

	prevTime = 0.00001f;

	return true;
}


bool SYSTEM::InitDXENGINE()
{
	// Create DXENGINE Objects
	m_DXENGINE = new DXENGINE;
	if ( !m_DXENGINE )
	{
		LOG_ERROR(" Failed - Create DXENGINE \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create DXENGINE \n ");
	}

	// Init DXENGINE Objects
	if ( !m_DXENGINE->Init( m_Width, m_Height, m_hWnd ) )
	{
		LOG_ERROR(" Failed - Init DXENGINE \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXENGINE \n ");
	}

	return true;
}

void SYSTEM::InitPointer()
{
	m_hWnd = nullptr;
	m_hInstance = NULL;
	m_WindowName = nullptr;
	m_WindowTitle = nullptr;
	m_DXENGINE = nullptr;

	m_SUBCPU = nullptr;
	m_SUBFPS = nullptr;
	m_SUBINPUT = nullptr;
	m_SUBTIMER = nullptr;
}


LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return g_SUBINPUT->MessageHandler( hWnd, message, wParam, lParam );
}