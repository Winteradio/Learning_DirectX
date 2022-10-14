#include "DXEngine.h"

DXENGINE::DXENGINE(){}

DXENGINE::DXENGINE( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	bool hr;

	hr = this->InitWindow( hInstance, nShowCmd, this->m_Width, this->m_Height, this->m_Fullscreen );
	if ( !hr )
	{
		LOG_ERROR(" Initalization Window - Failed \n ");
		this->Cleanup();
	}
	else
	{
		LOG_INFO(" Initalization Window - Successed \n ");
	}


	hr = this->InitD3D11();
	if ( !hr )
	{
		LOG_ERROR(" Initalization D3D11 - Failed \n ");
		this->Cleanup();
	}
	else
	{
		LOG_INFO(" Initalization D3D11 - Successed \n ");
	}
}

DXENGINE::~DXENGINE()
{
	Cleanup();
}

bool DXENGINE::InitD3D11()
{
	HRESULT hr;

	// Describe our Buffer
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory( &bufferDesc, sizeof( DXGI_MODE_DESC ) );

	bufferDesc.Width = this->m_Width;
	bufferDesc.Height = this->m_Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	// Describe out SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = this->m_hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	// Create our Device and SwapChain
	hr = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &(this->m_SwapChain), &(this->m_D3D11Device), NULL, &(this->m_D3D11DevCon) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create Device and SwapChain - Failed \n ");
		return false;
	}


	// Create the BackBuffer
	ID3D11Texture2D* backBuffer;

	hr = this->m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" SwapChain Get BackBuffer - Failed \n ");
		return false;
	}


	// Create the Render Target
	hr = this->m_D3D11Device->CreateRenderTargetView( backBuffer, NULL, &(this->m_RenderTargetView) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create the Render Target View - Failed \n ");
		return false;
	}

	backBuffer->Release();


	// Set the Render Target
	this->m_D3D11DevCon->OMSetRenderTargets( 1, &(this->m_RenderTargetView), NULL );

	return true;
}

void DXENGINE::MainLoop()
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
		else
		{
			// Game LOGIC
			UpdateScene();
			DrawScene();
		}
	}
}

void DXENGINE::Cleanup()
{
	m_SwapChain->Release();
	m_D3D11Device->Release();
	m_D3D11DevCon->Release();
}

bool DXENGINE::InitScene()
{

	return true;
}

bool DXENGINE::UpdateScene()
{
	// Update the colors of our scene
	this->red += this->colormodr * 0.00005f;
	this->green += this->colormodg * 0.00002f;
	this->blue += this->colormodb * 0.00001f;

	if ( red >= 0.1f || red <= 0.0f )
	{
		colormodr *= -1;
	}

	if ( green >= 0.1f || green <= 0.0f )
	{
		colormodg *= -1;
	}

	if ( blue >= 0.1f || blue <= 0.0f )
	{
		colormodb *= -1;
	}

	return true;
}

bool DXENGINE::DrawScene()
{
	// Clear our backbuffer to the updated color
	D3DXCOLOR bgColor( red, green, blue, 1.0f );

	this->m_D3D11DevCon->ClearRenderTargetView( this->m_RenderTargetView, bgColor );

	// Present the backbuffer to the screen
	this->m_SwapChain->Present( 0, 0 );

	return true;
}
