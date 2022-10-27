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

bool MDTEXTURE::Init( ID3D11Device* Device, ID3D11DeviceContext* DevContext, char* TextureFile )
{
	int Width, Height;

	Width = 0; Height = 0;

	// Load Data of Texture's image to memory
	if ( !LoadTexture( TextureFile, Width, Height ) )
	{
		LOG_ERROR(" Failed - Load Texture file \n ");
		return false;
	}

	// Describe Texture Structure
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory( &textureDesc, sizeof( D3D11_TEXTURE2D_DESC ) );



}

void MDTEXTURE::Release()
{
	m_TextureView->Release();
	m_Texture->Release();
	m_TextureData->Release();

	InitPointer();
}

ID3D11ShaderResourceView* MDTEXTURE::GetTexture() { return m_TextureView; }

void MDTEXTURE::InitPointer()
{
	m_TextureData = nullptr;
	m_Texture = nullptr;
	m_TextureView = nullptr;
}

bool MDTEXTURE::LoadTexture( char* TextureFIle, int& Width, int& Height )
{

}