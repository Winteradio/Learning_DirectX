#include "DXD3D.h"

DXD3D::DXD3D()
{
	InitPointer();
}


DXD3D::DXD3D( const DXD3D* Other )
{
	*this = *Other;
}


DXD3D::~DXD3D()
{}


bool DXD3D::Init( int Width, int Height, bool VSYNC, HWND hWnd, float SCREEN_DEPTH, float SCREEN_NEAR )
{
	if ( !InitSwapChain( Width, Height, hWnd ) ) { return false; }

	if ( !InitRenderTarget() ) { return false; }

	if ( !InitDepthStencil( Width, Height ) ) { return false; }

	if ( !InitRasterizer() ) { return false; }

	if ( !InitBlend() ) { return false; }

	InitViewport( Width, Height );
	InitMatrix( Width, Height, SCREEN_DEPTH, SCREEN_NEAR );

	return true;
}


void DXD3D::Release()
{
	if ( m_SwapChain ) { m_SwapChain->SetFullscreenState( false, NULL ); }

	m_SwapChain->Release();
	m_Device->Release();
	m_DeviceContext->Release();
	m_RenderTargetView->Release();
	m_DepthStencilBuffer->Release();
	m_DepthStencilView->Release();
	m_RasterStateSL->Release();
	m_RasterStateWF->Release();

	InitPointer();
}


void DXD3D::BeginScene( float Red, float Blue, float Green, float Alpha )
{
	// Set Color that erase Buffer
	float Color[4] = { Red, Green, Blue, Alpha };

	// Erase Backbuffer
	m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, Color );

	// Erase Depthbuffer
	m_DeviceContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}


void DXD3D::EndScene()
{
	m_SwapChain->Present( 0, 0 );
}


bool DXD3D::InitSwapChain( int Width, int Height, HWND hWnd )
{
	HRESULT hr;

	// Describe our Buffer
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( DXGI_MODE_DESC ) );

	bufferDesc.Width = Width;
	bufferDesc.Height = Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Create Device and SwapChain
	hr = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext );

	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Device and SwapChain \n");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create Device and SwapChain \n");
		return true;
	}

}


bool DXD3D::InitRenderTarget()
{
	HRESULT hr;

	// Create the Backbuffer
	ID3D11Texture2D* backBuffer;

	hr = m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Backbuffer \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create Backbuffer \n ");
	}

	// Create the Render Target View
	hr = m_Device->CreateRenderTargetView( backBuffer, NULL, &m_RenderTargetView);
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Render Target View \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create Render Target View \n ");
	}

	backBuffer->Release();

	// Set the Render Target
	m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, NULL );

	return true;
}


bool DXD3D::InitDepthStencil( int Width, int Height )
{
	HRESULT hr;

	// Create Depth Buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory( &depthBufferDesc, sizeof( D3D11_TEXTURE2D_DESC ) );

	depthBufferDesc.Width = Width;
	depthBufferDesc.Height = Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = m_Device->CreateTexture2D( &depthBufferDesc, NULL, &m_DepthStencilBuffer );
	if ( FAILED ( hr ) )
	{
		LOG_ERROR(" Failed - Create Depth/Stencil buffer \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create Depth/Stencil buffer \n ");
	}

	// Create Depth/Stencil State
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthEnabledStencilState );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Enabled Depth/Stencil State \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Enabled Depth/Stencil State \n ");
	}

	depthStencilDesc.DepthEnable = false;

	hr = m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthDisabledStencilState );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Disabled Depth/Stencil State \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Disabled Depth/Stencil State \n ");
	}

	m_DeviceContext->OMSetDepthStencilState( m_DepthEnabledStencilState, 1 );


	// Create Depth/Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory( &depthStencilViewDesc, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_Device->CreateDepthStencilView( m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView );
	if ( FAILED ( hr ) )
	{
		LOG_ERROR(" Failed - Create Depth/Stencil View \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Succsed - Create Depth/Stencil View \n ");
	}

	// Set Depth/Stencil View to Render Target
	m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, m_DepthStencilView );

	return true;
}

bool DXD3D::InitRasterizer()
{
	HRESULT hr;

	// Describe the  Rasterizer
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );

    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

    hr = m_Device->CreateRasterizerState( &rasterDesc, &m_RasterStateWF );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Create Raster State - WireFrame ");
    	return false;
    }
    else
    {
    	LOG_INFO(" Successed - Create Rasterizer State - WireFrame ");
    }

    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    hr = m_Device->CreateRasterizerState( &rasterDesc, &m_RasterStateSL );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Create Rasterizer State - Solid \n ");
        return false;
    }
    else
    {
    	LOG_INFO(" Succssed - Create Rasterizer State - Solid \n ");
    }
    return true;
}


void DXD3D::InitViewport( int Width, int Height )
{
	// Describe the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT ) );

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)Width;
	viewport.Height = (float)Height;

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// Set viewports
	m_DeviceContext->RSSetViewports( 1, &viewport );
}


void DXD3D::InitMatrix( int Width, int Height, float SCREEN_DEPTH, float SCREEN_NEAR )
{
    float fieldOfView = 3.141592654f / 4.0f;
    float screenAspect = (float)Width / (float)Height;

    // Set for 3D Rendering
    m_ProjectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

    // Init World View Matrix
    m_WorldMatrix = XMMatrixIdentity();

    // Set for 2D Rendering
    m_OrthoMatrix = XMMatrixOrthographicLH((float)Width, (float)Height, SCREEN_NEAR, SCREEN_DEPTH);
}


void DXD3D::InitPointer()
{
		IDXGISwapChain* m_SwapChain = nullptr;
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11Texture2D* m_DepthStencilBuffer = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		ID3D11RasterizerState* m_RasterState = nullptr;
}

bool DXD3D::InitBlend()
{
	HRESULT hr;

	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory( &blendStateDesc, sizeof( D3D11_BLEND_DESC ) );

	D3D11_RENDER_TARGET_BLEND_DESC renderTargetblendDesc;
	ZeroMemory( &renderTargetblendDesc, sizeof( D3D11_RENDER_TARGET_BLEND_DESC ) );

	renderTargetblendDesc.BlendEnable = true;
	renderTargetblendDesc.SrcBlend = D3D11_BLEND_ONE;
	renderTargetblendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	renderTargetblendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	renderTargetblendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	renderTargetblendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	renderTargetblendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	renderTargetblendDesc.RenderTargetWriteMask = 0x0f;

	blendStateDesc.RenderTarget[0] = renderTargetblendDesc;

	hr = m_Device->CreateBlendState( &blendStateDesc, &m_AlphaEnabledBlendingState);
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Enabled Blend State \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Enabled Blend State \n ");
	}

	blendStateDesc.RenderTarget[0].BlendEnable = false;

	hr = m_Device->CreateBlendState( &blendStateDesc, &m_AlphaDisabledBlendingState );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Disabled Blend State \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Disabled Blend State \n ");
	}

	return true;
}


ID3D11Device* DXD3D::GetDevice() { return m_Device; }

ID3D11DeviceContext* DXD3D::GetDeviceContext() { return m_DeviceContext; }

void DXD3D::GetProjectionMatrix( XMMATRIX& ProjectionMartix ) { ProjectionMartix = m_ProjectionMatrix; }

void DXD3D::GetWorldMatrix( XMMATRIX& WorldMatrix ) { WorldMatrix = m_WorldMatrix; }

void DXD3D::GetOrthoMatrix( XMMATRIX& OrthoMatrix ) { OrthoMatrix = m_OrthoMatrix; }

void DXD3D::TurnWireFrameOn()
{
	m_DeviceContext->RSSetState( m_RasterStateWF );
}

void DXD3D::TurnWireFrameOff()
{
	m_DeviceContext->RSSetState( m_RasterStateSL );
}

void DXD3D::TurnZBufferOn() { m_DeviceContext->OMSetDepthStencilState( m_DepthEnabledStencilState, 1 ); }

void DXD3D::TurnZBufferOff() { m_DeviceContext->OMSetDepthStencilState( m_DepthDisabledStencilState, 1 ); }

void DXD3D::TurnOnAlphaBlending()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	m_DeviceContext->OMSetBlendState( m_AlphaEnabledBlendingState, blendFactor, 0xffffffff );
}

void DXD3D::TurnOffAlphaBlending()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	m_DeviceContext->OMSetBlendState( m_AlphaDisabledBlendingState, blendFactor, 0xffffffff );
}
