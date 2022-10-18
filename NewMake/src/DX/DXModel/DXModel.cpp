#include "DXModel.h"


DXMODEL::DXMODEL()
{
	InitPointer();
}


DXMODEL::DXMODEL( const DXMODEL* Other )
{
	*this = *Other;
}


DXMODEL::~DXMODEL(){};


bool DXMODEL::Init( ID3D11Device* Device )
{
	if ( !InitVertex( Device ) ) { return  false; }
	if ( !InitIndex( Device ) ) { return false; }

	return true;
}


void DXMODEL::Release()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	InitPointer();
}


void DXMODEL::Render( ID3D11DeviceContext* DevContext )
{
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


bool DXMODEL::InitVertex( ID3D11Device* Device )
{
	HRESULT hr;

	// Set Vertex Count;
	m_VertexCount = 3;

	// Create Vertex List
	VertexType* vertices = new VertexType[ m_VertexCount ];
	if ( !vertices )
	{
		return false;
	}

	// Set Vertex List Data
	vertices[0].POS = XMFLOAT3( -1.0f, -1.0f, 0.0f );
	vertices[0].COLOR = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );

	vertices[1].POS = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	vertices[1].COLOR = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );

	vertices[2].POS = XMFLOAT3( 1.0f, -1.0f, 0.0f );
	vertices[2].COLOR = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );

	// Describe VertexBuffer Structor that is static
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Present Pointer about Vertex Data to Subresouce Structor
    D3D11_SUBRESOURCE_DATA vertexData;
    ZeroMemory( &vertexData, sizeof( D3D11_SUBRESOURCE_DATA ) );

    vertexData.pSysMem = vertices;
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

    delete[] vertices;
    vertices = 0;

    return true;
}


bool DXMODEL::InitIndex( ID3D11Device* Device )
{
	HRESULT hr;

	// Set Index Count;
	m_IndexCount = 3;

	// Create Index List
	UINT* indices = new UINT[ m_IndexCount ];
	if ( !indices )
	{
		return false;
	}

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	// Describe Index Buffer that is static
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof( indices ) * m_IndexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;


    // Describe Subresouce Structor that have Pointer for Index Data
    D3D11_SUBRESOURCE_DATA indexData;
    ZeroMemory( &indexData, sizeof( D3D11_SUBRESOURCE_DATA ) );

    indexData.pSysMem = indices;
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

    delete[] indices;
    indices = nullptr;

    return true;
}


void DXMODEL::InitPointer()
{
	m_VertexBuffer = nullptr;
	m_IndexBuffer = nullptr;
}


int DXMODEL::GetIndexCount() { return m_IndexCount; }