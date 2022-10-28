#include "DXT_Shader.h"

DXT_SHADER::DXT_SHADER()
{
	InitPointer();
}



DXT_SHADER::DXT_SHADER( const DXT_SHADER* Other )
{
	*this = *Other;
}

DXT_SHADER::~DXT_SHADER(){}



bool DXT_SHADER::Init( ID3D11Device* Device, ID3D11DeviceContext* DevContext, const char* VSfileDIR, const char* PSfileDIR )
{
	if ( !InitVertexShader( Device, DevContext, VSfileDIR ) ) { return false; }
	if ( !InitPixelShader( Device, DevContext, PSfileDIR ) ) { return false; }
	if ( !InitLayout( Device ) ) { return false; }
	if ( !InitSampleState( Device ) ) { return false; }
	if ( !InitConstantBuffer( Device ) ) { return false; }
	if ( !InitPixelBuffer( Device ) ) { return false; }

	return true;
}



void DXT_SHADER::Release()
{
	m_VertexShaderBuffer->Release();
	m_VertexShader->Release();

	m_PixelShaderBuffer->Release();
	m_PixelShader->Release();

	m_Layout->Release();

	m_SamplerState->Release();

	m_ConstantBuffer->Release();
	m_PixelBuffer->Release();

	InitPointer();
}



bool DXT_SHADER::Render( ID3D11DeviceContext* DevContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projMatrix, ID3D11ShaderResourceView* fontShaderView, XMFLOAT4 pixelColor )
{
	if ( !SetShaderParameters( DevContext, worldMatrix, viewMatrix, projMatrix, fontShaderView, pixelColor ) )
	{
		LOG_ERROR(" Failed - Set Shader Parameters \n ");
		return false;
	}

	DevContext->IASetInputLayout( m_Layout );
	DevContext->VSSetShader( m_VertexShader, NULL, 0 );
	DevContext->PSSetShader( m_PixelShader, NULL, 0 );
	DevContext->PSSetSamplers( 0, 1, &m_SamplerState );
	DevContext->DrawIndexed( indexCount, 0, 0 );

	return true;
}



bool DXT_SHADER::SetShaderParameters( ID3D11DeviceContext* DevContext,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projMatrix,
	ID3D11ShaderResourceView* fontShaderView, XMFLOAT4 pixelColor )
{
	HRESULT hr;

	////////////////
	// MATRIX BUFFER
	////////////////
	// Transpose matrixes for using Shader
	XMMATRIX world, view, proj;
	world = XMMatrixTranspose(worldMatrix);
	view = XMMatrixTranspose(viewMatrix);
	proj = XMMatrixTranspose(projMatrix);

	// Lock for using Constant Buffer's Information
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory( &mappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );

	hr = DevContext->Map( m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Close Constant Buffer \n ");
		return false;
	}

	// Get Pointer about Constant Buffer's Data
	ConstantBufferType* dataPtr = (ConstantBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;

	// Unlock Constant Buffer;
	DevContext->Unmap( m_ConstantBuffer, 0 );

	// Set Constant Buffer's Information in Vertex Shader
	UINT bufferNumer = 0;

	// Set Vertex Shader using Constant Buffer's Setting Value
	DevContext->VSSetConstantBuffers( bufferNumer, 1, &m_ConstantBuffer );
	DevContext->PSSetShaderResources( 0, 1, &fontShaderView );


	////////////////
	// PIXEL BUFFER
	////////////////
	hr = DevContext->Map( m_PixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Close Constant Buffer \n ");
		return false;
	}

	PixelBufferType* dataPtr2 = (PixelBufferType*)mappedResource.pData;

	dataPtr2->pixelColor = pixelColor;

	DevContext->Unmap( m_PixelBuffer, 0 );

	bufferNumer = 0;

	DevContext->PSSetConstantBuffers( bufferNumer, 1, &m_PixelBuffer );

	return true;
}



void DXT_SHADER::ShaderErrorMessage( ID3D10Blob* errorMessage )
{
	LOG_ERROR("%s \n",(char*)( errorMessage->GetBufferPointer() ) ) ;


	errorMessage->Release();
	errorMessage = nullptr;
}



bool DXT_SHADER::InitVertexShader( ID3D11Device* Device, ID3D11DeviceContext* DevContext, const char* VSfileDIR )
{
	HRESULT hr;
	ID3D10Blob* errorMessage = nullptr;

	hr = D3DX11CompileFromFile( VSfileDIR, 0, 0, "FontVertexShader", "vs_5_0", 0, 0, 0, &m_VertexShaderBuffer, &errorMessage, 0 );
	if ( FAILED( hr ) )
	{
		if ( errorMessage )
		{
			LOG_ERROR(" Failed - Compile %s \n ", VSfileDIR );
			ShaderErrorMessage( errorMessage );
		}
		else
		{
			LOG_ERROR(" Failed - Find %s \n ", VSfileDIR );
		}
	}
	else
	{
		LOG_INFO(" Successed - Compile %s \n ", VSfileDIR );
	}

	hr = Device->CreateVertexShader( m_VertexShaderBuffer->GetBufferPointer(), m_VertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Vertex Shader for Font \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Vertex Shader for Font \n ");
	}

	return true;
}



bool DXT_SHADER::InitPixelShader( ID3D11Device* Device, ID3D11DeviceContext* DevContext, const char* PSfileDIR )
{
	HRESULT hr;
	ID3D10Blob* errorMessage = nullptr;

	hr = D3DX11CompileFromFile( PSfileDIR, 0, 0, "FontPixelShader", "ps_5_0", 0, 0, 0, &m_PixelShaderBuffer, &errorMessage, 0 );
	if ( FAILED( hr ) )
	{
		if ( errorMessage )
		{
			LOG_ERROR(" Failed - Compile %s \n ", PSfileDIR );
			ShaderErrorMessage( errorMessage );
		}
		else
		{
			LOG_ERROR(" Failed - Find %s \n ", PSfileDIR );
		}
	}
	else
	{
		LOG_INFO(" Successed - Compile %s \n ", PSfileDIR );
	}

	hr = Device->CreatePixelShader( m_PixelShaderBuffer->GetBufferPointer(), m_PixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Pixel Shader for Font \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Pixel Shader for Font \n ");
	}

	return true;
}



bool DXT_SHADER::InitLayout( ID3D11Device* Device )
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

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
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
    m_VertexShaderBuffer = nullptr;

    m_PixelShaderBuffer->Release();
    m_PixelShaderBuffer = nullptr;

    return true;
}



bool DXT_SHADER::InitSampleState( ID3D11Device* Device )
{
	HRESULT hr;

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampleDesc;
	ZeroMemory( &sampleDesc, sizeof( D3D11_SAMPLER_DESC ) );

	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampleDesc.MipLODBias = 0.0f;
	sampleDesc.MaxAnisotropy = 1;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampleDesc.BorderColor[0] = 0;
	sampleDesc.BorderColor[1] = 0;
	sampleDesc.BorderColor[2] = 0;
	sampleDesc.BorderColor[3] = 0;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = Device->CreateSamplerState( &sampleDesc, &m_SamplerState );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Sample State for Texture \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Sample State for Texture \n ");
	}

	return true;
}



bool DXT_SHADER::InitConstantBuffer( ID3D11Device* Device )
{
	HRESULT hr;

    // Create Matrix Buffer in Vertex Shader
    D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory( &constantBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.ByteWidth = sizeof( ConstantBufferType );
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufferDesc.MiscFlags = 0;
    constantBufferDesc.StructureByteStride = 0;

    hr = Device->CreateBuffer( &constantBufferDesc, NULL, &m_ConstantBuffer );
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



bool DXT_SHADER::InitPixelBuffer( ID3D11Device* Device )
{
	HRESULT hr;

    // Create Matrix Buffer in Vertex Shader
    D3D11_BUFFER_DESC pixelBufferDesc;
    ZeroMemory( &pixelBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

    pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
    pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    pixelBufferDesc.MiscFlags = 0;
    pixelBufferDesc.StructureByteStride = 0;

    hr = Device->CreateBuffer( &pixelBufferDesc, NULL, &m_PixelBuffer );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Pixel Buffer in Vertex Shader \n ");
    	return false;
    }
    else
    {
    	LOG_INFO(" Successed - Pixel Buffer in Vertex Shader \n ");
    }

    return true;
}



void DXT_SHADER::InitPointer()
{
	m_VertexShaderBuffer = nullptr;
	m_VertexShader = nullptr;

	m_PixelShaderBuffer = nullptr;
	m_PixelShader = nullptr;

	m_Layout = nullptr;
	m_SamplerState = nullptr;

	m_ConstantBuffer = nullptr;
	m_PixelBuffer = nullptr;
}

