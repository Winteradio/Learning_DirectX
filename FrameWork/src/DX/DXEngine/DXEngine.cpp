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
	InitFileDIR();

	if ( !InitDXD3D( Width, Height, VSYNC_ENABLED, hWnd, SCREEN_DEPTH, SCREEN_NEAR ) ) { return false; }
	if ( !InitDXCAMERA() ) { return false; }
	if ( !InitDXLIGHT( m_LVSfileDIR, m_LPSfileDIR ) ) { return true; }
	if ( !InitDXMODEL( m_LIMGfileDIR, m_MDfileDIR ) ) { return false; }
	if ( !InitDXTEXT( Width, Height, m_TFontfileDIR, m_TDDSfileDIR, m_TVSfileDIR, m_TPSfileDIR ) ) { return false; }

	rotation = 0.0f;
	move = 0.0f;
	move_temp = 0.001f;

	return true;
}


bool DXENGINE::Frame( int FPS, int CPU, float Time, int mouseX, int mouseY )
{
	if ( !m_DXTEXT->Frame( m_DXD3D->GetDeviceContext(), mouseX, mouseY, CPU, FPS ) )
	{
		LOG_ERROR(" Failed - Print Mouse Position \n ");
		return false;
	}

	rotation += (float)XM_PI * 0.0001f;
	if ( rotation > 360.0f )
	{
		rotation -= 360.0f;
	}

	move += move_temp;
	if ( move <= -2.5f || move >= 2.5f )
	{
		move_temp *= -1.0f;
	}

	m_DXCAMERA->SetPosition( 0, 0.0f, -7.0f );

	return Render( rotation );
}


void DXENGINE::Release()
{
	m_DXD3D->Release();
	m_DXLIGHT->Release();
	m_DXMODEL->Release();
	m_DXTEXT->Release();

	delete m_DXD3D;
	delete m_DXCAMERA;
	delete m_DXLIGHT;
	delete m_DXMODEL;
	delete m_DXTEXT;

	InitPointer();
}


bool DXENGINE::Render( float rotation )
{
	bool hr;

	// Erase Buffer for Drawing Scene
	m_DXD3D->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );

	// Create View Matrix about Camera Position
	m_DXCAMERA->Render();

	// Get World, View, Proj Matrix from Camera and DXD3D Object
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	m_DXCAMERA->GetViewMatrix( viewMatrix );
	m_DXD3D->GetWorldMatrix( worldMatrix );
	m_DXD3D->GetProjectionMatrix( projectionMatrix );
	m_DXD3D->GetOrthoMatrix( orthoMatrix );


	worldMatrix = XMMatrixTranslation( 0.0f, 0.0f, move ) *XMMatrixRotationX( rotation);

	// Ready for Drawing tha need Model Vertex and Index buffer for Graphics pipeline
	m_DXMODEL->Render( m_DXD3D->GetDeviceContext() );

	m_DXD3D->TurnWireFrameOff();
	// Render using Shader
	hr = m_DXLIGHT->Render( m_DXD3D->GetDeviceContext(), m_DXMODEL->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_DXMODEL->GetTexture(), m_DXCAMERA->GetPosition() );
	if ( !hr )
	{
		LOG_ERROR(" Failed - Render using Shader \n ");
		return false;
	}

	m_DXD3D->GetWorldMatrix( worldMatrix );

	m_DXD3D->TurnZBufferOff();
	m_DXD3D->TurnOnAlphaBlending();

	m_DXD3D->TurnWireFrameOff();
	if ( !m_DXTEXT->Render( m_DXD3D->GetDeviceContext(), worldMatrix, orthoMatrix ) )
	{
		LOG_ERROR(" Failed - Render uisng Text \n ");
	}

	m_DXD3D->TurnOffAlphaBlending();
	m_DXD3D->TurnZBufferOn();

	// Print Buffor on Monitor
	m_DXD3D->EndScene();

	return true;
}


void DXENGINE::InitFileDIR()
{
	m_LVSfileDIR = ".\\..\\..\\src\\DX\\DXLight\\DXL_Shader\\Shader\\VSLight.hlsl";
	m_LPSfileDIR = ".\\..\\..\\src\\DX\\DXLight\\DXL_Shader\\Shader\\PSLight.hlsl";
	m_LIMGfileDIR = ".\\..\\..\\textures\\RedSquare.png";

	m_MDfileDIR = ".\\..\\..\\src\\DX\\DXModel\\DMText\\DMBox.txt";

	m_TVSfileDIR = ".\\..\\..\\src\\DX\\DXText\\DXT_Shader\\Shader\\VSFont.hlsl";
	m_TPSfileDIR = ".\\..\\..\\src\\DX\\DXText\\DXT_Shader\\Shader\\PSFont.hlsl";
	m_TFontfileDIR = ".\\..\\..\\src\\DX\\DXText\\DXT_Font\\DXT_Font.txt";
	m_TDDSfileDIR = ".\\..\\..\\src\\DX\\DXText\\DXT_Font\\DXT_Font.dds";
}


void DXENGINE::InitPointer()
{
	m_DXD3D = nullptr;
	m_DXCAMERA = nullptr;
	m_DXLIGHT= nullptr;
	m_DXMODEL= nullptr;

	m_LVSfileDIR = nullptr;
	m_LPSfileDIR = nullptr;
	m_LIMGfileDIR = nullptr;

	m_MDfileDIR = nullptr;

	m_TVSfileDIR = nullptr;
	m_TPSfileDIR = nullptr;
	m_TFontfileDIR = nullptr;
	m_TDDSfileDIR = nullptr;
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

	m_DXCAMERA->SetPosition( 0.0f, 0.0f, -1.0f );
	m_DXCAMERA->Render();
	return true;
}


bool DXENGINE::InitDXLIGHT( const char* VSfileDIR, const char* PSfileDIR )
{
	// Create DXLIGHT Object
	m_DXLIGHT = new DXLIGHT;

	if ( !m_DXLIGHT )
	{
		LOG_ERROR(" Failed - Create DXLIGHT \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXLIGHT \n ");
	}

	if ( !m_DXLIGHT->Init( m_DXD3D->GetDevice(), m_DXD3D->GetDeviceContext(), VSfileDIR, PSfileDIR ) )
	{
		LOG_ERROR(" Failed - Init DXLIGHT \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init DXLIGHT \n ");
	}

	m_DXLIGHT->SetAmbientColor( 0.5f, 0.5f, 0.5f, 1.0f );
	m_DXLIGHT->SetDiffuseColor( 0.75f, 0.75f, 0.75f, 1.0f );
	m_DXLIGHT->SetSpecularColor( 1.0f, 1.0f, 1.0f, 1.0f );
	m_DXLIGHT->SetSpecularPower( 32.0f );
	m_DXLIGHT->SetDirection( -1.0f, 1.0f, 1.0f );

	return true;
}


bool DXENGINE::InitDXMODEL( const char* TexfileDIR, const char* MDfileDIR )
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
	if ( !m_DXMODEL->Init( m_DXD3D->GetDevice(), TexfileDIR, MDfileDIR ) )
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


bool DXENGINE::InitDXTEXT( int screenWidth, int screenHeight, const char* FontfileDIR, const char* TexfileDIR, const char* VSfileDIR, const char* PSfileDIR )
{
	XMMATRIX baseViewMatrix;
	m_DXCAMERA->GetViewMatrix( baseViewMatrix );

	// Create DXTEXT Object
	m_DXTEXT = new DXTEXT;

	if ( !m_DXTEXT )
	{
		LOG_ERROR(" Failed - Create DXTEXT \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXTEXT \n ");
	}

	// Init DXTEXT Object
	if ( !m_DXTEXT->Init( m_DXD3D->GetDevice(), m_DXD3D->GetDeviceContext(), screenWidth, screenHeight, baseViewMatrix, FontfileDIR, TexfileDIR, VSfileDIR, PSfileDIR ) )
	{
		LOG_ERROR(" Failed - Init DXTEXT \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init DXTEXT \n ");
	}

	return true;
}
