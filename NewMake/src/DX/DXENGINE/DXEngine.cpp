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
	// Create Direct3D Object
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

	// Init Direct3D Object
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


bool DXENGINE::Frame()
{
	return Render();
}


void DXENGINE::Release()
{
	if ( m_DXD3D )
	{
		m_DXD3D->Release();
		m_DXD3D = nullptr;
	}
}


bool DXENGINE::Render()
{
	// Erase Buffer for Drawing Scene
	m_DXD3D->BeginScene( 0.0f, 0.5f, 0.5f, 1.0f );

	// Print Buffor on Monitor
	m_DXD3D->EndScene();

	return true;
}


void DXENGINE::InitPointer()
{
	m_DXD3D = nullptr;
}
