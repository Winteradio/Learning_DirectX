#include "DXT_Font.h"

#include <fstream>

using namespace std;

DXT_FONT::DXT_FONT()
{
	InitPointer();
}

DXT_FONT::DXT_FONT( const DXT_FONT* Other )
{
	*this = *Other;
}

DXT_FONT::~DXT_FONT(){}

bool DXT_FONT::Init( ID3D11Device* Device, const char* FontfileDIR, const char* DDSfileDIR )
{
	if ( !LoadFontData( FontfileDIR ) ) { return false; }
	if ( !LoadTexture( Device, DDSfileDIR ) ) { return false; }

	return true;
}

void DXT_FONT::Release()
{
	m_Texture->Release();

	if ( m_Font )
	{
		delete[] m_Font;
	}

	InitPointer();
}

void DXT_FONT::BuildVertexArray( void* vertices, const char* sentence, float drawX, float drawY )
{
	VertexType* vertexPtr = (VertexType*)vertices;

	int letter;
	int numLetters = (int)strlen(sentence);
	int index = 0;

	for ( int I = 0; I < numLetters; I++ )
	{
		letter = ( (int)sentence[ I ] ) - 32;

		if ( letter == 0 )
		{
			drawX += 3.0f;
		}
		else
		{
			// First triangle in quad
			vertexPtr[ index ].POS = XMFLOAT3( drawX, drawY, 0.0f );
			vertexPtr[ index ].TEXTURE = XMFLOAT2( m_Font[ letter ].left, 0.0f );
			index++;

			vertexPtr[ index ].POS = XMFLOAT3( drawX + m_Font[ letter ].size, drawY - 16, 0.0f );
			vertexPtr[ index ].TEXTURE = XMFLOAT2( m_Font[ letter ].right, 1.0f );
			index++;

			vertexPtr[ index ].POS = XMFLOAT3( drawX, drawY - 16, 0.0f );
			vertexPtr[ index ].TEXTURE = XMFLOAT2( m_Font[ letter ].left, 1.0f );
			index++;

			// Second triangle in quad
			vertexPtr[ index ].POS = XMFLOAT3( drawX, drawY, 0.0f );
			vertexPtr[ index ].TEXTURE = XMFLOAT2( m_Font[ letter ].left, 0.0f );
			index++;

			vertexPtr[ index ].POS = XMFLOAT3( drawX + m_Font[ letter ].size, drawY, 0.0f );
			vertexPtr[ index ].TEXTURE = XMFLOAT2( m_Font[ letter ].right, 0.0f );
			index++;

			vertexPtr[ index ].POS = XMFLOAT3( drawX + m_Font[ letter ].size, drawY - 16, 0.0f );
			vertexPtr[ index ].TEXTURE = XMFLOAT2( m_Font[ letter ].right, 1.0f );
			index++;

			// Update the X location for drawing by the size of the letter and one pixel
			drawX = drawX + m_Font[ letter ].size + 1.0f;
		}
	}
}

bool DXT_FONT::LoadFontData( const char* FontfileDIR )
{
	ifstream fin;

	m_Font = new FontType[ 95 ];
	if ( !m_Font )
	{
		LOG_ERROR(" Failed - Create FontType Array \n ");
		return false;
	}

	fin.open( FontfileDIR );
	if ( fin.fail() )
	{
		LOG_ERROR(" Failed - Open Font Text file \n ");
		return false;
	}


	char Temp;
	for ( int I = 0; I < 95; I++ )
	{
		fin.get( Temp );
		while ( Temp != ' ' )
		{
			fin.get( Temp );
		}

		fin.get( Temp );
		while ( Temp != ' ' )
		{
			fin.get( Temp );
		}

		fin >> m_Font[ I ].left;
		fin >> m_Font[ I ].right;
		fin >> m_Font[ I ].size;
		m_Font[ I ].size *= 2;
	}

	fin.close();

	return true;
}

bool DXT_FONT::LoadTexture( ID3D11Device* Device, const char* DDSfileDIR )
{
	// Create the texture object
	m_Texture = new DXTEXTURE;

	if ( !m_Texture )
	{
		LOG_ERROR(" Failed - Create Texture Object for Font \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Texture Object for Font \n ");
	}

	if ( !m_Texture->LoadTexture( Device, DDSfileDIR ) )
	{
		LOG_ERROR(" Failed - Init Texture Object for Font \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init Texture Object for Font \n ");
	}

	return true;
}

void DXT_FONT::InitPointer()
{
	m_Font = nullptr;
	m_Texture = nullptr;
}

ID3D11ShaderResourceView* DXT_FONT::GetTexture() { return m_Texture->GetTexture(); }