#include "DXEngine.h"

DXENGINE::DXENGINE()
{
	InitPointer();
}


DXENGINE::DXENGINE( const DXENGINE* Other )
{
	*this = *Other;
}


DXENGINE::~DXENGINE()
{}


bool DXENGINE::Init( int Width, int Height, HWND hWnd )
{
	if ( !InitDXD3D( Width, Height, VSYNC_ENABLED, hWnd, SCREEN_DEPTH, SCREEN_NEAR ) ) { return false; }
	if ( !InitDXCAMERA() ) { return false; }
	if ( !InitDXMODEL() ) { return false; }
	if ( !InitDXSHADER() ) { return false; }

	return true;
}


bool DXENGINE::Frame()
{
	return Render();
}


void DXENGINE::Release()
{
	m_DXD3D->Release();
	m_DXMODEL->Release();
	m_DXSHADER->Release();

	delete m_DXD3D;
	delete m_DXCAMERA;
	delete m_DXMODEL;
	delete m_DXSHADER;

	InitPointer();
}


bool DXENGINE::Render()
{
	bool hr;

	// Erase Buffer for Drawing Scene
	m_DXD3D->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );

	// Create View Matrix about Camera Position
	m_DXCAMERA->Render();

	// Get World, View, Proj Matrix from Camera and DXD3D Object
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_DXD3D->GetWorldMatrix( worldMatrix );
	m_DXCAMERA->GetViewMatrix( viewMatrix );
	m_DXD3D->GetProjectionMatrix( projectionMatrix );

	// Ready for Drawing tha need Model Vertex and Index buffer for Graphics pipeline
	m_DXMODEL->Render( m_DXD3D->GetDeviceContext() );

	// Render using Shader
	hr = m_DXSHADER->Render( m_DXD3D->GetDeviceContext(), m_DXMODEL->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix );
	if ( !hr )
	{
		LOG_ERROR(" Failed - Render using Shader \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Render using Shader \n ");
	}

	// Print Buffor on Monitor
	m_DXD3D->EndScene();

	return true;
}


void DXENGINE::InitPointer()
{
	m_DXD3D = nullptr;
	m_DXCAMERA = nullptr;
	m_DXMODEL= nullptr;
	m_DXSHADER = nullptr;
}


bool DXENGINE::InitDXD3D( int Width, int Height, bool VSYNC_ENABLED, HWND hWnd, float SCREEN_DEPTH, float SCREEN_NEAR )
{
	// Create DXD3D Object
	m_DXD3D = new DXD3D;

	if ( !m_DXD3D )
	{
		LOG_ERROR(" Failed - Create DXD3D \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create DXD3D \n ");
	}

	// Init DXD3D Object
	if ( !m_DXD3D->Init( Width, Height, VSYNC_ENABLED, hWnd, SCREEN_DEPTH, SCREEN_NEAR ) )
	{
		LOG_ERROR(" Failed - Init DXD3D \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Init DXD3D \n ");
	}

	return true;
}


bool DXENGINE::InitDXCAMERA()
{
	// Create DXCAMERA Object
	m_DXCAMERA = new DXCAMERA;

	if ( !m_DXCAMERA )
	{
		LOG_ERROR(" Failed - Create DXCAMERA \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXCAMERA \n ");
	}

	return true;
}


bool DXENGINE::InitDXMODEL()
{
	// Create DXMODEL Object
	m_DXMODEL = new DXMODEL;

	if ( !m_DXMODEL )
	{
		LOG_ERROR(" Failed - Create DXMODEL \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXMODEL \n ");
	}

	// Init DXMODEL Object
	if ( !m_DXMODEL->Init( m_DXD3D->GetDevice() ) )
	{
		LOG_ERROR(" Failed - Init DXMODEL \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init DXMODEL \n ");
	}

	return true;
}


bool DXENGINE::InitDXSHADER()
{
	// Create DXSHADER Object
	m_DXSHADER = new DXSHADER;

	if ( !m_DXSHADER )
	{
		LOG_ERROR(" Failed - Create DXSHADER \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXSHADER \n ");
	}

	// Init DXSHADER Object
	if ( !m_DXSHADER->Init( m_DXD3D->GetDevice(), m_DXD3D->GetDeviceContext() ) )
	{
		LOG_ERROR(" Failed - Init DXSHADER \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init DXSHADER \n");
	}

	return true;
}
