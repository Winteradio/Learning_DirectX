#include "DXLight.h"

DXLIGHT::DXLIGHT()
{
	InitPointer();
}

DXLIGHT::DXLIGHT( const DXLIGHT* Other )
{
	*this = *Other;
}

DXLIGHT::~DXLIGHT(){}

bool DXLIGHT::Init( ID3D11Device* Device, ID3D11DeviceContext* DevContext, const char* VSfileDIR, const char* PSfileDIR )
{
	if ( !InitLightShader( Device, DevContext, VSfileDIR, PSfileDIR ) ) { return false; }

	return true;
}

bool DXLIGHT::InitLightShader( ID3D11Device* Device, ID3D11DeviceContext* DevContext, const char* VSfileDIR, const char* PSfileDIR )
{
	// Create Light Shader
	m_LightShader = new DXL_SHADER;
	if ( !m_LightShader )
	{
		LOG_ERROR(" Failed - Create Light Shader \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Light Shader \n ");
	}

	// Initialize Light Shader
	if ( !m_LightShader->Init( Device, DevContext, VSfileDIR, PSfileDIR ) )
	{
		LOG_ERROR(" Failed - Init Light Shader \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init Light Shader \n ");
	}

	return true;
}

bool DXLIGHT::Render( ID3D11DeviceContext* DevContext, int indexCount,
	XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	ID3D11ShaderResourceView* lightShaderView, XMFLOAT3 cameraPosition )
{
	if ( !m_LightShader->Render( DevContext, indexCount,
		world, view, proj,
		lightShaderView,
		m_Direction, m_DiffuseColor, cameraPosition ) )
	{
		LOG_ERROR(" Failed - Render using Light Shader \n ");
		return false;
	}

	return true;
}

void DXLIGHT::Release()
{
	m_LightShader->Release();
	delete m_LightShader;

	InitPointer();
}

void DXLIGHT::InitPointer()
{
	m_LightShader = nullptr;
}

void DXLIGHT::SetAmbientColor( float X, float Y, float Z, float W )
{
	m_AmbientColor = XMFLOAT4( X, Y, Z, W );
}

void DXLIGHT::SetDiffuseColor( float X, float Y, float Z, float W )
{
	m_DiffuseColor = XMFLOAT4( X, Y, Z ,W );
}

void DXLIGHT::SetDirection( float X, float Y, float Z )
{
	m_Direction = XMFLOAT3( X, Y ,Z );
}

void DXLIGHT::SetSpecularColor( float X, float Y, float Z, float W )
{
	m_SpecularColor = XMFLOAT4( X, Y ,Z ,W );
}

void DXLIGHT::SetSpecularPower( float X )
{
	m_SpecularPower = X;
}

XMFLOAT4 DXLIGHT::GetAmbientColor() { return m_AmbientColor; }

XMFLOAT4 DXLIGHT::GetDiffuseColor() { return m_DiffuseColor; }

XMFLOAT3 DXLIGHT::GetDirection() { return m_Direction; }

XMFLOAT4 DXLIGHT::GetSpecularColor() { return m_SpecularColor; }

float DXLIGHT::GetSpecularPower() { return m_SpecularPower; }