#include "DXEngine.h"

DXENGINE::DXENGINE(){}

DXENGINE::DXENGINE( const DXENGINE& Other )
{
	*this = Other;
}

DXENGINE::~DXENGINE(){}

bool DXENGINE::Init( int Width, int Height, HWND hWnd )
{
	// Create Direct3D Object
	m_DXD3D = new DXD3D;

	if ( !m_DXD3D )
	{
		LOG_ERROR("Create Direct3D - Failed \n");
		return false;
	}

	// Init Direct3D Object
	if ( !m_DXD3D->Init( Width, Height, VSYNC_ENABLED, hWnd, SCREEN_DEPTH, SCREEN_NEAR ) )
	{
		LOG_ERROR(" Init Direct3D - Failed \n");
		return false;
	}

	return true;
}

bool DXENGINE::Frame()
{
	return Render();
}

void DXENGINE::Done()
{
	if ( m_DXD3D )
	{
		m_DXD3D->Done();
		m_DXD3D = nullptr;
	}
}

bool DXENGINE::Render()
{
	// Erase Buffer for Drawing Scene
	m_DXD3D->BeginScene( 0.5f, 0.5f, 0.5f, 1.0f );

	// Print Buffor on Monitor
	m_DXD3D->EndScene();

	return true;
}
