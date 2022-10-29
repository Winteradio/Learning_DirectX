#include "DXTexture.h"

DXTEXTURE::DXTEXTURE()
{
	InitPointer();
}

DXTEXTURE::DXTEXTURE( const DXTEXTURE* Other )
{
	*this = *Other;
}

DXTEXTURE::~DXTEXTURE() {}

void DXTEXTURE::Release()
{
	m_TextureView->Release();

	InitPointer();
}

void DXTEXTURE::InitPointer()
{
	m_TextureView = nullptr;
}

bool DXTEXTURE::LoadTexture( ID3D11Device* Device, const char* fileDIR )
{
	HRESULT hr;

	hr = D3DX11CreateShaderResourceViewFromFile( Device, fileDIR, NULL, NULL, &m_TextureView, NULL );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Compile DDS Texture file %s \n ", fileDIR );
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Compile DDS Texture file %s \n ", fileDIR );
	}

	return true;
}


ID3D11ShaderResourceView* DXTEXTURE::GetTexture() { return m_TextureView; }