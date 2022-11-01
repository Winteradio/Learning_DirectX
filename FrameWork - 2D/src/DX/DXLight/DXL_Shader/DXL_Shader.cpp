#include "DXL_Shader.h"


DXL_SHADER::DXL_SHADER()
{
	InitPointer();
}

DXL_SHADER::DXL_SHADER( const DXL_SHADER* Other )
{
	*this = *Other;
}

DXL_SHADER::~DXL_SHADER()
{}


bool DXL_SHADER::Init( ID3D11Device* Device, ID3D11DeviceContext* DevContext, const char* VSfileDIR, const char* PSfileDIR )
{
	m_ShaderSet = DXLCOLOR;

	if ( !InitVertexShader( Device, DevContext, VSfileDIR ) ) { return false; }
	if ( !InitPixelShader( Device, DevContext, PSfileDIR ) ) { return false; }
	if ( !InitLayout( Device ) ) { return false; }
	if ( !InitMatrixBuffer( Device ) ) { return false; }
	if ( !InitLightBuffer( Device ) ) { return false; }
	if ( !InitCameraBuffer( Device ) ) { return false; }

	if ( m_ShaderSet == DXLTEXTURE )
	{
		if ( !InitSampleState( Device ) ) { return false; }
	}

	return true;
}


void DXL_SHADER::Release()
{
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_Layout->Release();

	m_SamplerState->Release();

	m_MatrixBuffer->Release();
	m_LightBuffer->Release();
	m_CameraBuffer->Release();

	InitPointer();
}


bool DXL_SHADER::Render( ID3D11DeviceContext* DevContext, int indexCount,
	XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	ID3D11ShaderResourceView* textureView,
	XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT4 specularColor, float specularPower, XMFLOAT3 cameraPosition )
{
	if ( !SetShaderParameters(
		DevContext,
		world, view, proj,
		textureView,
		lightDirection, diffuseColor, ambientColor, specularColor, specularPower, cameraPosition ) )
	{
		LOG_ERROR(" Failed - Set Shader Parameters \n ");
		return false;
	}
	DevContext->IASetInputLayout( m_Layout );
	DevContext->VSSetShader( m_VertexShader, NULL, 0 );
	DevContext->PSSetShader( m_PixelShader, NULL, 0 );

	if ( m_ShaderSet == DXLTEXTURE )
	{
		DevContext->PSSetSamplers( 0, 1, &m_SamplerState );
	}

	DevContext->DrawIndexed( indexCount, 0, 0 );

	return true;
}


bool DXL_SHADER::SetShaderParameters( ID3D11DeviceContext* DevContext,
	XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	ID3D11ShaderResourceView* textureView,
	XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT4 specularColor, float specularPower, XMFLOAT3 cameraPosition )
{
	HRESULT hr;

	////////////////
	// MATRIX BUFFER
	////////////////
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


	////////////////
	// CAMERA BUFFER
	////////////////
	hr = DevContext->Map( m_CameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Close Constant Buffer \n ");
		return false;
	}

	CameraBufferType* dataPtr3 = (CameraBufferType*)mappedResource.pData;

	dataPtr3->cameraPosition = cameraPosition;
	dataPtr3->padding = 0.0f;

	DevContext->Unmap( m_CameraBuffer, 0 );

	bufferNumer = 1;

	DevContext->VSSetConstantBuffers( bufferNumer, 1 , &m_CameraBuffer );
	DevContext->PSSetShaderResources( 0, 1, &textureView );


	////////////////
	// LIGHT BUFFER
	////////////////
	hr = DevContext->Map( m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Close Constant Buffer \n ");
		return false;
	}

	// Get Pointer about Constant Buffer's Data
	LightBufferType* dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->specularColor = specularColor;
	dataPtr2->specularPower = specularPower;

	// Unlock Constant Buffer;
	DevContext->Unmap( m_LightBuffer, 0 );

	// Set Constant Buffer's Information in Vertex Shader
	bufferNumer = 0;

	// Set Pixel Shader using Constant Buffer's Setting Value
	DevContext->PSSetConstantBuffers( bufferNumer, 1, &m_LightBuffer );

	return true;
}



void DXL_SHADER::ShaderErrorMessage( ID3D10Blob* errorMessage )
{
	LOG_ERROR("%s \n",(char*)( errorMessage->GetBufferPointer() ) ) ;


	errorMessage->Release();
	errorMessage = nullptr;
}


bool DXL_SHADER::InitVertexShader( ID3D11Device* Device, ID3D11DeviceContext* DevContext, const char* VSfileDIR )
{
	HRESULT hr;
	ID3D10Blob* errorMessage = nullptr;


	// Compile Vertex Shader Code File
	hr = D3DX11CompileFromFile( VSfileDIR, 0, 0, "ColorVertexShader", "vs_5_0", D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY, 0, 0, &m_VertexShaderBuffer, &errorMessage, 0 );
	if ( FAILED( hr ) )
	{
		if ( errorMessage )
		{
			LOG_ERROR(" Failed - Compile %s \n ", VSfileDIR );
			ShaderErrorMessage( errorMessage );
			system("pause");
		}
		else
		{
			LOG_ERROR(" Failed - Find %s \n ", VSfileDIR );
		}

		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Compile %s \n ", VSfileDIR );
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
		LOG_INFO(" Succssed - Create Vertex Shader from Buffer \n ");
	}

	return true;
}



bool DXL_SHADER::InitPixelShader( ID3D11Device* Device, ID3D11DeviceContext* DevContext, const char* PSfileDIR )
{
	HRESULT hr;
	ID3D10Blob* errorMessage = nullptr;

	// Compile Pixel Shader Code File
	hr = D3DX11CompileFromFile( PSfileDIR, 0, 0, "ColorPixelShader", "ps_5_0", 0, 0, 0, &m_PixelShaderBuffer, &errorMessage, 0 );
	if ( FAILED( hr ) )
	{
		if ( errorMessage )
		{
			LOG_ERROR(" Failed - Compile %s \n ", PSfileDIR );
			ShaderErrorMessage( errorMessage );
			system("pause");
		}
		else
		{
			LOG_ERROR(" Failed - Find %s \n ", PSfileDIR );
		}

		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Compile %s \n ", PSfileDIR );
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


bool DXL_SHADER::InitLayout( ID3D11Device* Device )
{
	HRESULT hr;

	// Create Layout for Vertex Input
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[2].InstanceDataStepRate = 0;

    switch( m_ShaderSet )
    {
    	case DXLTEXTURE :
    	{
    		polygonLayout[1].SemanticName = "TEXCOORD";
		    polygonLayout[1].SemanticIndex = 0;
		    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		    polygonLayout[1].InputSlot = 0;
		    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		    polygonLayout[1].InstanceDataStepRate = 0;
    		break;
    	}
    	case DXLCOLOR :
    	{
    		polygonLayout[1].SemanticName = "COLOR";
		    polygonLayout[1].SemanticIndex = 0;
		    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		    polygonLayout[1].InputSlot = 0;
		    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		    polygonLayout[1].InstanceDataStepRate = 0;
    		break;
    	}
    }

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

bool DXL_SHADER::InitSampleState( ID3D11Device* Device )
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



bool DXL_SHADER::InitMatrixBuffer( ID3D11Device* Device )
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

bool DXL_SHADER::InitLightBuffer( ID3D11Device* Device )
{
	HRESULT hr;

	// Create Light Buffer in Pixel Shader
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory( &lightBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof( LightBufferType );
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	hr = Device->CreateBuffer( &lightBufferDesc, NULL, &m_LightBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Light Buffer \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Light Buffer \n ");
	}

	return true;
}

bool DXL_SHADER::InitCameraBuffer( ID3D11Device* Device )
{
	HRESULT hr;

	// Create Camera Buffer in Pixel Shader
	D3D11_BUFFER_DESC cameraBufferDesc;
	ZeroMemory( &cameraBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof( CameraBufferType );
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	hr = Device->CreateBuffer( &cameraBufferDesc, NULL, &m_CameraBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Camera Buffer \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Camera Buffer \n ");
	}

	return true;
}


void DXL_SHADER::InitPointer()
{
	m_VertexShaderBuffer = nullptr;
	m_VertexShader = nullptr;
	m_PixelShaderBuffer = nullptr;
	m_PixelShader = nullptr;

	m_Layout = nullptr;
	m_SamplerState = nullptr;

	m_MatrixBuffer = nullptr;
	m_LightBuffer = nullptr;
	m_CameraBuffer = nullptr;
}