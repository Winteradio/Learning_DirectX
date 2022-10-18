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

	hr = this->InitScene();
	if ( !hr )
	{
		LOG_ERROR(" Initalization Scene - Failed \n ");
		this->Cleanup();
	}
	else
	{
		LOG_INFO(" Initalization Scene - Successed \n ");
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


	// Create Depth/Stencil Buffer Description
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	ZeroMemory( &depthStencilDesc, sizeof( D3D11_TEXTURE2D_DESC ) );

	depthStencilDesc.Width = this->m_Width;
	depthStencilDesc.Height = this->m_Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	this->m_D3D11Device->CreateTexture2D( &depthStencilDesc, NULL, &(this->m_DepthStencilBuffer) );
	this->m_D3D11Device->CreateDepthStencilView( this->m_DepthStencilBuffer, NULL, &(this->m_DepthStencilView) );


	// Create the Render Target
	hr = this->m_D3D11Device->CreateRenderTargetView( backBuffer, NULL, &(this->m_RenderTargetView) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create the Render Target View - Failed \n ");
		return false;
	}

	backBuffer->Release();


	// Set the Render Target
	this->m_D3D11DevCon->OMSetRenderTargets( 1, &(this->m_RenderTargetView), this->m_DepthStencilView );

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
	m_RenderTargetView->Release();

	m_VS->Release();
	m_PS->Release();
	m_VS_Buffer->Release();
	m_PS_Buffer->Release();
	m_VertLayout->Release();

	m_SquareIndexBuffer->Release();
	m_SquareVertBuffer->Release();

	m_DepthStencilView->Release();
	m_DepthStencilBuffer->Release();

	m_WireFrames->Release();

	m_cbPerObjectBuffer->Release();
	m_CubesTexture->Release();
	m_CubesTexSamplerState->Release();

	m_Transparency->Release();
	m_CCWcullmode->Release();
	m_CWcullmode->Release();
}

bool DXENGINE::InitScene()
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE( Layout );

	this->VertexShader = ".\\..\\..\\shader\\VertexShader.hlsl";
	this->PixelShader = ".\\..\\..\\shader\\PixelShader.hlsl";
	this->Shader = ".\\..\\..\\shader\\Shader.hlsl";
	this->Texture = ".\\..\\..\\textures\\braynzar.jpg";

	ID3D10Blob* errorMessage = nullptr;
// Compile Shaders from shader file
	hr = D3DX11CompileFromFile( this->Shader, 0, 0, "VS", "vs_4_0", 0, 0, 0, &(this->m_VS_Buffer), &errorMessage, 0 );
	if ( FAILED( hr ) )
	{
		if ( errorMessage )
		{
			LOG_ERROR(" Compile From Vertex Shader file - Failed \n ");
		}
		else
		{
			LOG_ERROR(" Not Found Vertex \n");
		}
		//return false;
	}
	else
	{
		LOG_INFO(" Compile From Vertex Shader file - Successed \n ");
	}

	hr = D3DX11CompileFromFile( this->Shader, 0, 0, "PS", "ps_4_0", 0, 0, 0, &(this->m_PS_Buffer), 0, 0 );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Compile From Pixel Shader file - Failed \n ");
		//return false;
	}
	else
	{
		LOG_INFO(" Compile From Pixel Shader file - Successed \n ");
	}

	// Create the Shader Objects
	hr = this->m_D3D11Device->CreateVertexShader( this->m_VS_Buffer->GetBufferPointer(), this->m_VS_Buffer->GetBufferSize(), NULL, &(this->m_VS) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create Vertex Shader file - Failed \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Create Vertex Shader file - Successed \n ");
	}

	hr = this->m_D3D11Device->CreatePixelShader( this->m_PS_Buffer->GetBufferPointer(), this->m_PS_Buffer->GetBufferSize(), NULL, &(this->m_PS) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create Pixel Shader file - Failed \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Create Pixel Shader file - Successed \n ");
	}


	// Set Vertex and Pixel Shaders
	this->m_D3D11DevCon->VSSetShader( this->m_VS, 0, 0 );
	this->m_D3D11DevCon->PSSetShader( this->m_PS, 0, 0 );


// Create the Vertex buffer
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		Vertex( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		Vertex( 1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		Vertex( 1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex( 1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Right Face
		Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
		Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
	};
	// Describe Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc;

	ZeroMemory( &vertexBufferDesc, sizeof( vertexBufferDesc ) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( Vertex ) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory( &vertexBufferData, sizeof( vertexBufferData ) );

	vertexBufferData.pSysMem = v;

	hr = this->m_D3D11Device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &(this->m_SquareVertBuffer ) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create Square Vertex Buffer - Failed \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Create Square Vertex Buffer - Succssed \n ");
	}


	// Set the Vertex Buffer;
	UINT stride = sizeof( Vertex );
	UINT offset = 0;
	this->m_D3D11DevCon->IASetVertexBuffers( 0, 1, &(this->m_SquareVertBuffer), &stride, &offset );


// Create the Index buffer
	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};


	// Describe the Index buffer
	D3D11_BUFFER_DESC indexBufferDesc;

	ZeroMemory( &indexBufferDesc, sizeof( indexBufferDesc ) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof( DWORD ) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;

	hr = this->m_D3D11Device->CreateBuffer( &indexBufferDesc, &iinitData, &(this->m_SquareIndexBuffer) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create Index Buffer - Failed \n");
	}
	else
	{
		LOG_INFO(" Create Index Buffer - Succssed \n");
	}

	this->m_D3D11DevCon->IASetIndexBuffer( this->m_SquareIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

// Create the Input Layout
	this->m_D3D11Device->CreateInputLayout( Layout, numElements, this->m_VS_Buffer->GetBufferPointer(), this->m_VS_Buffer->GetBufferSize(), &(this->m_VertLayout) );


	// Set the Input Layout
	this->m_D3D11DevCon->IASetInputLayout( this->m_VertLayout );


	// Set Primitive Topology
	this->m_D3D11DevCon->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

// Set Rasterizer
	D3D11_RASTERIZER_DESC wfdesc;

	ZeroMemory( &wfdesc, sizeof( D3D11_RASTERIZER_DESC ) );

	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;

	hr = this->m_D3D11Device->CreateRasterizerState( &wfdesc, &(this->m_WireFrames) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create Wire Frame - Failed \n");
	}
	else
	{
		LOG_INFO(" Create Wire Frame - Successed \n");
	}

	this->m_D3D11DevCon->RSSetState( this->m_WireFrames );

// Create the Viewport
	D3D11_VIEWPORT viewport;

	ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT) );

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)this->m_Width;
	viewport.Height = (float)this->m_Height;

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;


	// Set the Viewport
	this->m_D3D11DevCon->RSSetViewports( 1, &viewport );


// Create the buffer to send to the cbuffer in effect file
	D3D11_BUFFER_DESC cbbd;

	ZeroMemory( &cbbd, sizeof( D3D11_BUFFER_DESC ) );

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof( cbPerObject );
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = this->m_D3D11Device->CreateBuffer( &cbbd, NULL,&(this->m_cbPerObjectBuffer) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create Constant Buffer - Failed \n ");
	}
	else
	{
		LOG_INFO(" Create Constant Buffer - Successed \n" );
	}

// Camera Information
	camPosition = XMVectorSet( 0.0f, 3.0f, -8.0f, 0.0f );
	camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	camUp = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	// Set the View matrix
	camView = XMMatrixLookAtLH( camPosition, camTarget, camUp );

	// Set the Projection Matrix
	camProjection = XMMatrixPerspectiveFovLH( 0.4f*3.14f, (float)this->m_Width / (float)this->m_Height, 1.0f, 1000.0f );

// Descirbe the Sample State
	hr = D3DX11CreateShaderResourceViewFromFile( this->m_D3D11Device, this->Texture, NULL, NULL, &(this->m_CubesTexture), NULL );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Compile Shader Resource View from Image File - Failed \n");
	}
	else
	{
		LOG_INFO(" Compile Shader Resource View from Image File - Successed \n");
	}

	D3D11_SAMPLER_DESC sampDesc;

	ZeroMemory( &sampDesc, sizeof( sampDesc ) );

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = this->m_D3D11Device->CreateSamplerState( &sampDesc, &(this->m_CubesTexSamplerState) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create Sample State - Failed \n");
	}
	else
	{
		LOG_INFO(" Create Sample State - Successed \n");
	}

// Blending Equation
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory( &rtbd, sizeof( rtbd ) );

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	this->m_D3D11Device->CreateBlendState( &blendDesc, &(this->m_Transparency) );


// CW & CWW Culling
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory( &cmdesc, sizeof( cmdesc ) );

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = true;

	hr = this->m_D3D11Device->CreateRasterizerState( &cmdesc, &(this->m_CCWcullmode) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create CCW Rasterizer State - Failed \n");
	}

	cmdesc.FrontCounterClockwise = false;
	hr = this->m_D3D11Device->CreateRasterizerState( &cmdesc, &(this->m_CWcullmode) );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Create CCW Rasterizer State - Failed \n");
	}

	return true;
}

bool DXENGINE::UpdateScene()
{
	// Keep the cubes rotating
	rot += 0.0005f;
	if ( rot >= 6.26f )
	{
		rot = 0.0f;
	}

	LOG_INFO("Rotation : %d \n",rot);

	// Reset cube1World
	cube1World = XMMatrixIdentity();

	// Define cube1's world space matrix
	XMVECTOR rotaxis = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	Rotation = XMMatrixRotationAxis( rotaxis, rot );
	Translation = XMMatrixTranslation( 0.0f, 0.0f, 4.0f );

	// Set Cube1's World space using the transformations
	cube1World = Translation * Rotation;

	// Reset Cube2world
	cube2World = XMMatrixIdentity();

	// Define Cube1's world space matrix
	Rotation = XMMatrixRotationAxis( rotaxis, -rot );
	Scale = XMMatrixScaling( 1.3f, 1.3f, 1.3f );

	// Set Cube'2 world space matrix
	cube2World = Rotation * Scale;
	return true;
}

bool DXENGINE::DrawScene()
{
	// Clear our Backbuffer
	float bgColor[4] = { ( 0.0f, 0.0f, 0.0f, 0.0f ) };
	this->m_D3D11DevCon->ClearRenderTargetView( this->m_RenderTargetView, bgColor );


	// Refresh the Depth/Stencil View
	this->m_D3D11DevCon->ClearDepthStencilView( this->m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

// Blend
	float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };

	this->m_D3D11DevCon->OMSetBlendState( 0, 0, 0xffffffff );

	this->m_D3D11DevCon->OMSetBlendState( this->m_Transparency, blendFactor, 0xffffffff );

	XMVECTOR cubePos = XMVectorZero();

	cubePos = XMVector3TransformCoord(cubePos, cube1World);

	float distX = XMVectorGetX(cubePos) - XMVectorGetX(camPosition);
	float distY = XMVectorGetY(cubePos) - XMVectorGetY(camPosition);
	float distZ = XMVectorGetZ(cubePos) - XMVectorGetZ(camPosition);

	float cube1Dist = distX*distX + distY*distY + distZ*distZ;

	//Find distance from second cube to camera
	cubePos = XMVectorZero();

	cubePos = XMVector3TransformCoord(cubePos, cube2World);

	distX = XMVectorGetX(cubePos) - XMVectorGetX(camPosition);
	distY = XMVectorGetY(cubePos) - XMVectorGetY(camPosition);
	distZ = XMVectorGetZ(cubePos) - XMVectorGetZ(camPosition);

	float cube2Dist = distX*distX + distY*distY + distZ*distZ;

	//If the first cubes distance is less than the second cubes

	if(cube1Dist < cube2Dist)
	{
		//Switch the order in which the cubes are drawn
		XMMATRIX tempMatrix = cube1World;
		cube1World = cube2World;
		cube2World = tempMatrix;
	}



	// Set the World / View / Projection Martrix, then Sned it to constant buffer in effect file
	// Cube1
	this->WVP = cube1World * this->camView * this->camProjection;

	cbPerObj.WVP = XMMatrixTranspose( WVP );

	this->m_D3D11DevCon->UpdateSubresource( this->m_cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );

	this->m_D3D11DevCon->VSSetConstantBuffers( 0, 1, &(this->m_cbPerObjectBuffer) );
// Texture
	this->m_D3D11DevCon->PSSetShaderResources( 0, 1, &(this->m_CubesTexture) );
	this->m_D3D11DevCon->PSSetSamplers( 0, 1, &(this->m_CubesTexSamplerState) );

	this->m_D3D11DevCon->RSSetState(this->m_CCWcullmode);
	this->m_D3D11DevCon->DrawIndexed( 36, 0, 0 );

	this->m_D3D11DevCon->RSSetState(this->m_CWcullmode);
	this->m_D3D11DevCon->DrawIndexed( 36, 0, 0 );
	// Cube2
	this->WVP = cube2World * this->camView * this->camProjection;

	cbPerObj.WVP = XMMatrixTranspose( WVP );

	this->m_D3D11DevCon->UpdateSubresource( this->m_cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );

	this->m_D3D11DevCon->VSSetConstantBuffers( 0, 1, &(this->m_cbPerObjectBuffer) );
// Texture
	this->m_D3D11DevCon->PSSetShaderResources( 0, 1, &(this->m_CubesTexture) );
	this->m_D3D11DevCon->PSSetSamplers( 0, 1, &(this->m_CubesTexSamplerState) );

	this->m_D3D11DevCon->RSSetState(this->m_CCWcullmode);
	this->m_D3D11DevCon->DrawIndexed( 36, 0, 0 );

	this->m_D3D11DevCon->RSSetState(this->m_CWcullmode);
	this->m_D3D11DevCon->DrawIndexed( 36, 0, 0 );


	// Present the Backbuffer to the screen
	this->m_SwapChain->Present( 0, 0 );

	return true;
}
