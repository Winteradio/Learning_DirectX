#include "DXModel.h"

#include <string>
#include <fstream>

using namespace std;


DXMODEL::DXMODEL()
{
	InitPointer();
}


DXMODEL::DXMODEL( const DXMODEL* Other )
{
	*this = *Other;
}


DXMODEL::~DXMODEL(){};


bool DXMODEL::Init( ID3D11Device* Device,
	int screenWidth, int screenHeight,
	char* textureFile,
	int bitmapWidth, int bitmapHeight )
{
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	m_BitmapWidth = bitmapWidth;
	m_BitmapHeight = bitmapHeight;

	m_PreviousPosX = -1;
	m_PreviousPosY = -1;

	if ( !LoadTexture( Device, textureFile ) ) { return false; }

	if ( !SetVertex() ) { return  false; }
	if ( !InitVertexBuffer( Device ) ) { return false; }
	if ( !SetIndex() ) { return false; }
	if ( !InitIndexBuffer( Device ) ) { return false; }

	return true;
}


void DXMODEL::Release()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_Texture->Release();

	InitPointer();

	if ( m_Model )
	{
		delete[] m_Model;
	}
}


void DXMODEL::Render( ID3D11DeviceContext* DevContext, int positionX, int positionY )
{
	Update( DevContext, positionX, positionY );

	// Set Vertex Type of Degree and offset
	UINT stride = sizeof( VertexType );
	UINT offset = 0;

	// Activate Vertex Buffer on IA for Rendering
	DevContext->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// Activate Index Buffer on IA for Rendering
	DevContext->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// Set Base Drawing Line using Vertex Buffer
	DevContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool DXMODEL::Update( ID3D11DeviceContext* DevContext, int positionX, int positionY )
{
	float left, right, top, bottom;

	if ( ( positionX == m_PreviousPosX ) && ( positionY == m_PreviousPosY ) )
	{
		return true;
	}

	m_PreviousPosX = positionX;
	m_PreviousPosY = positionY;

	left = (float)( ( m_ScreenWidth / 2 ) * ( -1 ) ) + (float)positionX;
	right = left + (float)m_BitmapWidth;
	top = (float)( m_ScreenHeight / 2 ) - (float)positionY;
	bottom = top - (float)m_BitmapHeight;

	m_Vertices[0].POS = XMFLOAT3( left, top, 0.0f );
	m_Vertices[0].TEXTURE = XMFLOAT2( 0.0f, 0.0f );

	m_Vertices[1].POS = XMFLOAT3( right, bottom, 0.0f );
	m_Vertices[1].TEXTURE = XMFLOAT2( 1.0f, 1.0f );

	m_Vertices[2].POS = XMFLOAT3( left, bottom, 0.0f );
	m_Vertices[2].TEXTURE = XMFLOAT2( 0.0f, 0.0f );

	m_Vertices[3].POS = XMFLOAT3( left, top, 0.0f );
	m_Vertices[3].TEXTURE = XMFLOAT2( 0.0f, 0.0f );

	m_Vertices[4].POS = XMFLOAT3( right, top, 0.0f );
	m_Vertices[4].TEXTURE = XMFLOAT2( 1.0f, 0.0f );

	m_Vertices[5].POS = XMFLOAT3( right, bottom, 0.0f );
	m_Vertices[5].TEXTURE = XMFLOAT2( 1.0f, 1.0f );

	if ( !UpdateVertexBuffer( DevContext ) ) { return false; }
	if ( !UpdateIndexBuffer( DevContext ) ) { return false; }

	return true;
}

bool DXMODEL::SetVertex()
{
	m_VertexCount = 6;

	// Create Vertex List
	m_Vertices = new VertexType[ m_VertexCount ];
	if ( !m_Vertices )
	{
		LOG_ERROR(" Failed - Create Vertex list \n ");
		return false;
	}

	return true;
}


bool DXMODEL::SetIndex()
{
	m_IndexCount = m_VertexCount;

	// Create Index List
	m_Indices = new UINT[ m_IndexCount ];
	if ( !m_Indices )
	{
		LOG_ERROR(" Failed - Create Index list \n ");
		return false;
	}

	for ( int I = 0; I < m_IndexCount; I++ )
	{
		m_Indices[ I ] = I;
	}

	return true;
}


bool DXMODEL::InitVertexBuffer( ID3D11Device* Device )
{
	HRESULT hr;

	// Describe VertexBuffer Structor that is static
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof( VertexType ) * m_VertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Present Pointer about Vertex Data to Subresouce Structor
    D3D11_SUBRESOURCE_DATA vertexData;
    ZeroMemory( &vertexData, sizeof( D3D11_SUBRESOURCE_DATA ) );

    vertexData.pSysMem = m_Vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Create Vertex Buffer;
    hr = Device->CreateBuffer( &vertexBufferDesc, &vertexData, &m_VertexBuffer );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Create Vertex Buffer \n ");
    	return false;
    }
    else
    {
    	LOG_INFO(" Succssed - Create Vertex Buffer \n ");
    }
    return true;
}


bool DXMODEL::InitIndexBuffer( ID3D11Device* Device )
{
	HRESULT hr;

	// Describe Index Buffer that is static
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

    indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    indexBufferDesc.ByteWidth = sizeof( UINT ) * m_IndexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;


    // Describe Subresouce Structor that have Pointer for Index Data
    D3D11_SUBRESOURCE_DATA indexData;
    ZeroMemory( &indexData, sizeof( D3D11_SUBRESOURCE_DATA ) );

    indexData.pSysMem = m_Indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;


    // Create Index Buffer
    hr = Device->CreateBuffer( &indexBufferDesc, &indexData, &m_IndexBuffer );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Create Index Data \n ");
    	return false;
    }
    else
    {
    	LOG_INFO(" Succssed - Create Index Data \n ");
    }

    return true;
}


bool DXMODEL::UpdateVertexBuffer( ID3D11DeviceContext* DevContext )
{
	HRESULT hr;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory( &mappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );

	hr = DevContext->Map( m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Close Vertex Buffer \n ");
		return false;
	}
	memcpy( mappedResource.pData, m_Vertices, sizeof( VertexType ) * m_VertexCount );
	DevContext->Unmap( m_VertexBuffer, 0 );

	return true;
}


bool DXMODEL::UpdateIndexBuffer( ID3D11DeviceContext* DevContext )
{
	HRESULT hr;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory( &mappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );

	hr = DevContext->Map( m_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - CLose Index Buffer \n ");
		return false;
	}
	memcpy( mappedResource.pData, m_Indices, sizeof( UINT ) * m_IndexCount );
	DevContext->Unmap( m_IndexBuffer, 0 );

	return true;
}


void DXMODEL::InitPointer()
{
	m_VertexBuffer = nullptr;
	m_IndexBuffer = nullptr;

	m_Vertices = nullptr;
	m_Indices = nullptr;

	m_Texture = nullptr;

	m_Model = nullptr;
}


bool DXMODEL::LoadTexture( ID3D11Device* Device )
{
	m_Texture = new MDTEXTURE();

	if ( !m_Texture )
	{
		LOG_ERROR(" Failed - Create Texture Object \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Texture Object \n ");
	}

	if ( !m_Texture->Init( Device ) )
	{
		LOG_ERROR(" Failed - Init Texture Object \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init Texture Object \n ");
	}

	return true;
}

int DXMODEL::GetIndexCount() { return m_IndexCount; }

ID3D11ShaderResourceView* DXMODEL::GetTexture() { return m_Texture->GetTexture(); }