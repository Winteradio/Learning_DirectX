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

	InitPointer();
}


void DXMODEL::Render( ID3D11DeviceContext* DevContext )
{
	Update( DevContext );

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

bool DXMODEL::Update( ID3D11DeviceContext* DevContext )
{
	if ( Red < 0.0f || Red > 1.0f ) { temp_R *= -1; }
	if ( Blue < 0.0f || Blue > 1.0f ) { temp_B *= -1; }
	if ( Green < 0.0f || Green > 1.0f ) { temp_G *= -1; }
	Red += temp_R;
	Blue += temp_B;
	Green += temp_G;

	SetVertex();
	SetIndex();
	if ( !UpdateVertexBuffer( DevContext ) ) { return false; }
	if ( !UpdateIndexBuffer( DevContext ) ) { return false; }

	return true;
}

bool DXMODEL::SetVertex()
{
	// Set Vertex Count;
	m_VertexCount = 3;

	// Create Vertex List
	m_Vertices = new VertexType[ m_VertexCount ];
	if ( !m_Vertices )
	{
		LOG_ERROR(" Failed - Create Vertex list \n ");
		return false;
	}

	// Set Vertex List Data
	m_Vertices[0].POS = XMFLOAT3( -1.0f, -1.0f, 0.0f );
	m_Vertices[0].COLOR = XMFLOAT4( Red, 0.0f, 0.0f, 1.0f );

	m_Vertices[1].POS = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	m_Vertices[1].COLOR = XMFLOAT4( 0.0f, 0.0f, Blue, 1.0f );

	m_Vertices[2].POS = XMFLOAT3( 1.0f, -1.0f, 0.0f );
	m_Vertices[2].COLOR = XMFLOAT4( 0.0f, Green, 0.0f, 1.0f );

	return true;
}


bool DXMODEL::SetIndex()
{
	// Set Index Count;
	m_IndexCount = 3;

	// Create Index List
	m_Indices = new UINT[ m_IndexCount ];
	if ( !m_Indices )
	{
		LOG_ERROR(" Failed - Create Index list \n ");
		return false;
	}

	m_Indices[0] = 0;
	m_Indices[1] = 1;
	m_Indices[2] = 2;

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

	Red = 0.0f;
	Blue = 0.0f;
	Green = 0.0f;

	temp_R = 0.0005f;
	temp_B = -0.0002f;
	temp_G = 0.0003f;
}


int DXMODEL::GetIndexCount() { return m_IndexCount; }