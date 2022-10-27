#include "MDTexture.h"

MDTEXTURE::MDTEXTURE()
{
	InitPointer();
}

MDTEXTURE::MDTEXTURE( const MDTEXTURE* Other )
{
	*this = *Other;
}

MDTEXTURE::~MDTEXTURE() {}

bool MDTEXTURE::Init( ID3D11Device* Device )
{
	m_TextureFile =  ".\\..\\..\\textures\\RedSquare.png";

	if ( !LoadTexture( Device ) ) { return false; }

	return true;
}

void MDTEXTURE::Release()
{
	m_TextureView->Release();

	InitPointer();
}

void MDTEXTURE::InitPointer()
{
	m_TextureView = nullptr;
	m_TextureFile = nullptr;
}

bool MDTEXTURE::LoadTexture( ID3D11Device* Device )
{
	HRESULT hr;

	hr = D3DX11CreateShaderResourceViewFromFile( Device, m_TextureFile, NULL, NULL, &m_TextureView, NULL );

	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Compile Shader REsourve View from Image File \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Compile Shader Resource View rom Image File \n ");
	}

	return true;
}

ID3D11ShaderResourceView* MDTEXTURE::GetTexture() { return m_TextureView; }