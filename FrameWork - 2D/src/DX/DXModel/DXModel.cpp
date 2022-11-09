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
	if ( !InitDXMMANGER( DXMCIRCLE ) ) { return false; }
	if ( !InitVertexBuffer( Device, 1 ) ) { return false; }
	if ( !InitIndexBuffer( Device, 1 ) ) { return false; }

	return true;
}


void DXMODEL::Release()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_DXTEXTURE->Release();
	m_DXMMANGER->Release( m_DXMODELLIST );

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
	UINT stride = sizeof( VERTEXINFO );
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


bool DXMODEL::Frame( bool InsertState, int mouseX, int mouseY, int Time, int prevTime )
{
	if ( !m_DXMMANGER ->Frame( m_DXMODELLIST, InsertState, mouseX, mouseY, Time, prevTime ) )
	{
		LOG_INFO(" Failed - Frame for DXMODEL \n ");
		return false;
	}
	return true;
}


bool DXMODEL::InitVertexBuffer( ID3D11Device* Device, int Num )
{
	HRESULT hr;

	// Describe VertexBuffer Structor that is static
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof( VERTEXINFO ) * GetVertexCount( Num );
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Present Pointer about Vertex Data to Subresouce Structor
    D3D11_SUBRESOURCE_DATA vertexData;
    ZeroMemory( &vertexData, sizeof( D3D11_SUBRESOURCE_DATA ) );

    vertexData.pSysMem = m_DXMODELLIST[ Num-1 ].VERTICES;
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


bool DXMODEL::InitIndexBuffer( ID3D11Device* Device, int Num )
{
	HRESULT hr;

	// Describe Index Buffer that is static
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

    indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    indexBufferDesc.ByteWidth = sizeof( UINT ) * GetIndexCount( Num );
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;


    // Describe Subresouce Structor that have Pointer for Index Data
    D3D11_SUBRESOURCE_DATA indexData;
    ZeroMemory( &indexData, sizeof( D3D11_SUBRESOURCE_DATA ) );

    indexData.pSysMem = m_DXMODELLIST[ Num-1 ].INDICES;
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


bool DXMODEL::InitDXMMANGER( DXMPOLYGON Type )
{
	m_MaxCount = 4000;

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

	if ( !m_DXMMANGER->Init( m_DXMODELLIST, m_MaxCount, Type ) )
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

void DXMODEL::InitPointer()
{
	m_VertexBuffer = nullptr;
	m_IndexBuffer = nullptr;

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

int DXMODEL::GetVertexCount( int Num ) { return m_DXMODELLIST[ Num - 1 ].NumVertex; }
int DXMODEL::GetIndexCount( int Num ) { return m_DXMODELLIST[ Num - 1 ].NumIndex; }
int DXMODEL::GetNumModel( int Num ) { return m_DXMODELLIST[ Num -1 ].NumModel; }
XMFLOAT3 DXMODEL::GetPosition( int Num, int Index ) { return m_DXMODELLIST[ Num -1 ].MODELS[ Index ].POS; }

ID3D11ShaderResourceView* DXMODEL::GetTexture() { return m_DXTEXTURE->GetTexture(); }