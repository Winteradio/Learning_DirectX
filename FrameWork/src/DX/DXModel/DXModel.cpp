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


bool DXMODEL::Init( ID3D11Device* Device, const char* IMGfileDIR, const char* MDfileDIR )
{
	if ( !LoadTexture( Device, IMGfileDIR ) ) { return false; }
	if ( !LoadModel( MDfileDIR ) ) { return false; }

	if ( !SetVertex() ) { return  false; }
	if ( !InitVertexBuffer( Device ) ) { return false; }
	if ( !SetIndex() ) { return false; }
	if ( !InitIndexBuffer( Device ) ) { return false; }
	//if ( !InitOutputBuffer( Device ) ) { return false; }

	if ( !InitDXMMANGER( m_Vertices, m_Indices, 3 ) ) { return false; }

	return true;
}


void DXMODEL::Release()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_DXTEXTURE->Release();
	m_DXMMANGER->Release();

	InitPointer();

	if ( m_ModelTXT )
	{
		delete[] m_ModelTXT;
	}
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
	//SetVertex();
	//SetIndex();
	//if ( !UpdateVertexBuffer( DevContext ) ) { return false; }
	//if ( !UpdateIndexBuffer( DevContext ) ) { return false; }
	//if ( !UpdateOutputBuffer( DevContext ) ) { return false; }

	return true;
}

bool DXMODEL::SetVertex()
{
	// Create Vertex List
	m_Vertices = new VertexType[ m_VertexCount ];
	if ( !m_Vertices )
	{
		LOG_ERROR(" Failed - Create Vertex list \n ");
		return false;
	}

	for ( int I = 0; I < m_VertexCount; I++ )
	{
		m_Vertices[ I ].POS = XMFLOAT3( m_ModelTXT[ I ].X, m_ModelTXT[ I ].Y, m_ModelTXT[ I ].Z );
		m_Vertices[ I ].TEXTURE = XMFLOAT2( m_ModelTXT[ I ].TU, m_ModelTXT[ I ].TV );
		m_Vertices[ I ].NORMAL = XMFLOAT3( m_ModelTXT[ I ].NX, m_ModelTXT[ I ].NY, m_ModelTXT[ I ].NZ );
	}

	return true;
}


bool DXMODEL::SetIndex()
{
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


bool DXMODEL::InitOutputBuffer( ID3D11Device* Device )
{
	HRESULT hr;

	// Describe output Buffer that is static
	D3D11_BUFFER_DESC outputBufferDesc;
	ZeroMemory( &outputBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

    outputBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    outputBufferDesc.ByteWidth = sizeof( VertexType ) * m_VertexCount;
    outputBufferDesc.BindFlags = D3D11_BIND_STREAM_OUTPUT;
    outputBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    outputBufferDesc.MiscFlags = 0;
    outputBufferDesc.StructureByteStride = 0;

    // Create output Buffer
    hr = Device->CreateBuffer( &outputBufferDesc, NULL, &m_OutputBuffer );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Create output Data \n ");
    	return false;
    }
    else
    {
    	LOG_INFO(" Succssed - Create output Data \n ");
    }

    return true;
}


bool DXMODEL::InitDXMMANGER( VertexType* vertices, UINT* indices, int numModel )
{
	m_DXMMANGER = new DXM_MANAGER;
	if ( !m_DXMMANGER )
	{
		LOG_ERROR(" Failed - Create DXM Manager Object \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXM Manager Object \n ");
	}

	if ( !m_DXMMANGER->Init( vertices, indices, m_VertexCount, numModel ) )
	{
		LOG_ERROR(" Failed - Init DXM Magner Object \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXM Manger Object \n ");
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


bool DXMODEL::UpdateOutputBuffer( ID3D11DeviceContext* DevContext )
{
	HRESULT hr;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory( &mappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );

	hr = DevContext->Map( m_OutputBuffer, 0, D3D11_MAP_READ, 0, &mappedResource );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - CLose Index Buffer \n ");
		return false;
	}
	memcpy( mappedResource.pData, m_Vertices, sizeof( UINT ) * m_VertexCount );
	DevContext->Unmap( m_OutputBuffer, 0 );

	return true;
}


void DXMODEL::InitPointer()
{
	m_VertexBuffer = nullptr;
	m_IndexBuffer = nullptr;

	m_Vertices = nullptr;
	m_Indices = nullptr;

	m_DXTEXTURE = nullptr;
	m_DXMMANGER = nullptr;

	m_ModelTXT = nullptr;
}


bool DXMODEL::LoadTexture( ID3D11Device* Device, const char* IMGfileDIR )
{
	m_DXTEXTURE = new DXTEXTURE();

	if ( !m_DXTEXTURE )
	{
		LOG_ERROR(" Failed - Create Texture Object \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Texture Object \n ");
	}

	if ( !m_DXTEXTURE->LoadTexture( Device, IMGfileDIR ) )
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


bool DXMODEL::LoadModel( const char* MDfileDIR )
{
	// Open Model Text file
	ifstream fin( MDfileDIR );

	// Check the whether file is opened
	if ( fin.fail() )
	{
		LOG_ERROR(" Failed - Open Model File %s \n ", MDfileDIR );
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Open Model File %s \n ", MDfileDIR );
	}

	// Get Value of VertexCount before Data
	char input;
	fin.get( input );
	for (int I = 0; I < 999; I ++)
	{
		if ( input == ':')
		{
			break;
		}
		fin.get( input );
	}
	// Read Vertex Count
	fin >> m_VertexCount;
	LOG_INFO(" MDfile Vertex Count : %d \n", m_VertexCount );

	// Set Index Count is same as Vertex Count
	m_IndexCount = m_VertexCount;

	// Create ModelTXT array
	m_ModelTXT = new ModelTXT[ m_VertexCount ];
	if ( !m_ModelTXT )
	{
		LOG_ERROR(" Failed - Create ModelTXT Array \n ");
		return false;
	}


	// Get Value of Data Before the file is end
	fin.get( input );
	for (int I = 0; I < 999; I ++)
	{
		if ( input == ':')
		{
			break;
		}
		fin.get( input );
	}
	fin.get( input );
	fin.get( input ); // Move to the Next line

	// Get Vertex Data
	for ( int I = 0 ; I < m_VertexCount; I++ )
	{
		fin >> m_ModelTXT[I].X >> m_ModelTXT[I].Y >> m_ModelTXT[I].Z;
		fin >> m_ModelTXT[I].TU >> m_ModelTXT[I].TV;
		fin >> m_ModelTXT[I].NX >> m_ModelTXT[I].NY >> m_ModelTXT[I].NZ;
	}

	// Close Model File
	fin.close();

	return true;
}


int DXMODEL::GetIndexCount() { return m_IndexCount; }

ID3D11ShaderResourceView* DXMODEL::GetTexture() { return m_DXTEXTURE->GetTexture(); }