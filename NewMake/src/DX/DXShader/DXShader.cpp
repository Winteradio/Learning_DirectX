#include "DXShader.h"


DXSHADER::DXSHADER()
{
	InitPointer();
}

DXSHADER::DXSHADER( const DXSHADER* Other )
{
	*this = *Other;
}

DXSHADER::~DXSHADER()
{}


bool DXSHADER::Init( ID3D11Device* Device )
{
	InitShaderDIR();
	if ( !InitVertexShader( Device ) ) { return false; }
	if ( !InitPixelShader( Device ) ) { return false; }
	if ( !InitLayout( Device ) ) { return false; }
	if ( !InitMatrixBuffer( Device ) ) { return false; }

	return true;
}


void DXSHADER::Release()
{
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_Layout->Release();
	m_MatrixBuffer->Release();

	InitPointer();
}


bool DXSHADER::Render( ID3D11DeviceContext* DevContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj )
{
	if ( !SetShaderParameters( DevContext, world, view, proj ) )
	{
		LOG_ERROR(" Failed - Set Shader Parameters \n ");
		return false;
	}

	DevContext->IASetInputLayout( m_Layout );

	DevContext->VSSetShader( m_VertexShader, NULL, 0 );
	DevContext->PSSetShader( m_PixelShader, NULL, 0 );

	DevContext->DrawIndexed( indexCount, 0, 0 );

	return true;
}


bool DXSHADER::SetShaderParameters( ID3D11DeviceContext* DevContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj )
{
	HRESULT hr;

	// Transpose matrixes for using Shader
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	// Lock for using Constant Buffer's Information
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory( &mappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );

	hr = DevContext->Map( m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Close Constant Buffer \n ");
		return false;
	}

	// Get Pointer about Constant Buffer's Data
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;

	// Unlock Constant Buffer;
	DevContext->Unmap( m_MatrixBuffer, 0 );

	// Set Constant Buffer's Information in Vertex Shader
	UINT bufferNumer = 0;

	// Set Vertex Shader using Constant Buffer's Setting Value
	DevContext->VSSetConstantBuffers( bufferNumer, 1, &m_MatrixBuffer );

	return true;
}


void DXSHADER::ShaderErrorMessage( ID3D10Blob* errorMessage )
{
	OutputDebugStringA(reinterpret_cast<const char*>( errorMessage->GetBufferPointer() ) ) ;


	errorMessage->Release();
	errorMessage = nullptr;
}


void DXSHADER::InitShaderDIR()
{
	// Set Shader Files Directory in CHAR Type
	m_VSfile = ".\\..\\..\\shader\\VertexShader.hlsl";
	m_PSfile = ".\\..\\..\\shader\\PixelShader.hlsl";
}


bool DXSHADER::InitVertexShader( ID3D11Device* Device )
{
	HRESULT hr;
	ID3D10Blob* errorMessage = nullptr;


	// Compile Vertex Shader Code File
	hr = D3DX11CompileFromFile( m_VSfile, 0, 0, "ColorVertexShader", "vs_5_0", 0, 0, 0, &m_VertexShaderBuffer, &errorMessage, 0 );
	if ( FAILED( hr ) )
	{
		if ( errorMessage )
		{
			LOG_ERROR(" Failed - Compile %s \n ", m_VSfile );
			ShaderErrorMessage( errorMessage );
		}
		else
		{
			LOG_ERROR(" Failed - Find %s \n ", m_VSfile );
		}

		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Compile %s \n ", m_VSfile );
	}


	// Create Vertex Shader from Buffer
	hr = Device->CreateVertexShader( m_VertexShaderBuffer->GetBufferPointer(), m_VertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Vertex Shader from Buffer \n ");
		return false;
	}
	else
	{
		LOG_INFO(" SUccssed - Create Vertex Shader from Buffer \n ");
	}

	return true;
}


bool DXSHADER::InitPixelShader( ID3D11Device* Device )
{
	HRESULT hr;
	ID3D10Blob* errorMessage = nullptr;

	// Compile Pixel Shader Code File
	hr = D3DX11CompileFromFile( m_PSfile, 0, 0, "ColorPixelShader", "ps_5_0", 0, 0, 0, &m_PixelShaderBuffer, &errorMessage, 0 );
	if ( FAILED( hr ) )
	{
		if ( errorMessage )
		{
			LOG_ERROR(" Failed - Compile %s \n ", m_PSfile );
			ShaderErrorMessage( errorMessage );
		}
		else
		{
			LOG_ERROR(" Failed - Find %s \n ", m_PSfile );
		}

		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Compile %s \n ", m_PSfile );
	}


	// Create Pixel Shader from Buffer
	hr = Device->CreatePixelShader( m_PixelShaderBuffer->GetBufferPointer(), m_PixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Pixel Shader from Buffer \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Pixel Shader from Buffer \n ");
	}

	return true;
}


bool DXSHADER::InitLayout( ID3D11Device* Device )
{
	HRESULT hr;

	// Create Layout for Vertex Input
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "COLOR";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    UINT numElements = ARRAYSIZE( polygonLayout );

    hr = Device->CreateInputLayout( polygonLayout, numElements, m_VertexShaderBuffer->GetBufferPointer(), m_VertexShaderBuffer->GetBufferSize(), &m_Layout );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Create Layout for Vertex Input \n ");
    	return false;
    }
    else
    {
    	LOG_INFO(" Successed - Create Layout for Vertex Input \n ");
    }

    m_VertexShaderBuffer->Release();
    m_VertexShaderBuffer = 0;

    m_PixelShaderBuffer->Release();
    m_PixelShaderBuffer = 0;

    return true;
}


bool DXSHADER::InitMatrixBuffer( ID3D11Device* Device )
{
	HRESULT hr;

    // Create Matrix Buffer in Vertex Shader
    D3D11_BUFFER_DESC matrixBufferDesc;
    ZeroMemory( &matrixBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    hr = Device->CreateBuffer( &matrixBufferDesc, NULL, &m_MatrixBuffer );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Matrix Buffer in Vertex Shader \n ");
    	return false;
    }
    else
    {
    	LOG_INFO(" Successed - Matrix Buffer in Vertex Shader \n ");
    }

    return true;
}


void DXSHADER::InitPointer()
{
	m_VertexShaderBuffer = nullptr;
	m_VertexShader = nullptr;
	m_PixelShaderBuffer = nullptr;
	m_PixelShader = nullptr;
	m_Layout = nullptr;
	m_MatrixBuffer = nullptr;

	m_VSfile = nullptr;
	m_PSfile = nullptr;
}