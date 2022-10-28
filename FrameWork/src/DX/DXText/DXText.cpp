#include "DXText.h"

DXTEXT::DXTEXT()
{
	InitPointer();
}


DXTEXT::DXTEXT( const DXTEXT* Other )
{
	*this = *Other;
}


DXTEXT::~DXTEXT() {}



bool DXTEXT::Init( ID3D11Device* Device, ID3D11DeviceContext* DevContext, int screenWidth, int screenHeight, XMMATRIX baseViewMatrix, const char* FontfileDIR, const char* TexfileDIR, const char* VSfileDIR, const char* PSfileDIR )
{
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	m_BaseViewMatrix = baseViewMatrix;

	if ( !InitFont( Device, FontfileDIR, TexfileDIR ) ) { return false; }
	if ( !InitFontShader( Device, DevContext, VSfileDIR, PSfileDIR ) ) { return false; }

	if ( !InitSentence( &m_Sentence1, 16, Device ) ) { return false; }
	if ( !InitSentence( &m_Sentence2, 16, Device ) ) { return false; }

	if ( !UpdateSentence( m_Sentence1, " Hello ", 100, 100, 1.0f, 1.0f, 1.0f, DevContext) ) { return false; }
	if ( !UpdateSentence( m_Sentence2, " GoodBye ", 100, 200, 1.0f, 1.0f, 0.0f, DevContext ) ) { return false; }

	return true;
}



void DXTEXT::Release()
{
	m_Font->Release();
	m_FontShader->Release();

	ReleaseSentence( &m_Sentence1 );
	ReleaseSentence( &m_Sentence2 );

	InitPointer();
}



void DXTEXT::ReleaseSentence( SentenceType** sentence )
{
	if ( *sentence )
	{
		if ( (*sentence)->vertexBuffer )
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		if ( (*sentence)->indexBuffer )
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		delete *sentence;
		*sentence = nullptr;
	}
}



bool DXTEXT::Render( ID3D11DeviceContext* DevContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix )
{
	if ( !RenderSentence( DevContext, m_Sentence1, worldMatrix, orthoMatrix ) ) { return false; }
	if ( !RenderSentence( DevContext, m_Sentence2, worldMatrix, orthoMatrix ) ) { return false; }

	return true;
}



bool DXTEXT::InitFont( ID3D11Device* Device, const char* FontfileDIR, const char* TexfileDIR )
{
	m_Font = new DXT_FONT;
	if ( !m_Font )
	{
		LOG_ERROR(" Failed - Create Font Object \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Font Object \n ");
	}



	if ( !m_Font->Init( Device, FontfileDIR, TexfileDIR ) )
	{
		LOG_ERROR(" Failed - Init Font Object \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init Font Object \n ");
	}

	return true;
}



bool DXTEXT::InitFontShader( ID3D11Device* Device, ID3D11DeviceContext* DevContext, const char* VSfileDIR, const char* PSfileDIR )
{
	m_FontShader = new DXT_SHADER;
	if ( !m_FontShader )
	{
		LOG_ERROR(" Failed - Create Font Shdaer Object \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Font Shader Object \n ");
	}


	if ( !m_FontShader->Init( Device, DevContext, VSfileDIR, PSfileDIR ) )
	{
		LOG_ERROR(" Failed - Init Font Shader Object \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init Font Shader Object \n ");
	}

	return true;
}



bool DXTEXT::InitSentence( SentenceType** sentence, int maxLength, ID3D11Device* Device )
{
	HRESULT hr;

	*sentence = new SentenceType;
	if ( !*sentence )
	{
		LOG_ERROR(" Failed - Create Sentence \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Succeessed - Create Sentence \n ");
	}

	(*sentence)->vertexBuffer = nullptr;
	(*sentence)->indexBuffer = nullptr;
	(*sentence)->maxLength = maxLength;
	(*sentence)->vertexCount = 6 * maxLength;
	(*sentence)->indexCount = (*sentence)->vertexCount;


	////////////////
	// VERTEX BUFFER
	////////////////
	VertexType* vertices = new VertexType[ (*sentence)->vertexCount ];
	if ( !vertices )
	{
		LOG_ERROR(" Failed - Create Vertex for Sentence \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Vertex for Sentence \n ");
	}

	memset( vertices, 0, ( sizeof( VertexType ) * (*sentence)->vertexCount ) );

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof( VertexType ) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory( &vertexData, sizeof( D3D11_SUBRESOURCE_DATA ) );

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	hr = Device->CreateBuffer( &vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Vertex Buffer \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Vertex Buffer \n ");
	}


	////////////////
	// INDEX BUFFER
	////////////////
	UINT* indices = new UINT[ (*sentence)->indexCount ];
	if ( !indices )
	{
		LOG_ERROR(" Failed - Create Index for Sentence \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Index for Sentence \n ");
	}

	for ( int I = 0; I < (*sentence)->indexCount; I++ )
	{
		indices[ I ] = I;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof( UINT ) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory( &indexData, sizeof( D3D11_SUBRESOURCE_DATA ) );

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hr = Device->CreateBuffer( &indexBufferDesc, &indexData, &(*sentence)->indexBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Index Buffer \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Index Buffer \n ");
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}



bool DXTEXT::UpdateSentence( SentenceType* sentence, char* text,
	int positionX, int positionY,
	float red, float blue ,float green,
	ID3D11DeviceContext* DevContext )
{
	HRESULT hr;

	sentence->Red = red;
	sentence->Blue = blue;
	sentence->Green = green;

	int numLetters = (int)strlen(text);

	if ( numLetters > sentence->maxLength )
	{
		LOG_ERROR(" Over the maxLength : %d that has Numbers of letters %d \n ", sentence->maxLength, numLetters );
		return false;
	}

	VertexType* vertices = new VertexType[ sentence->vertexCount ];
	if ( !vertices )
	{
		LOG_ERROR(" Failed - Create Vertex for Sentence \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Vertex for Sentence \n ");
	}

	memset( vertices, 0, ( sizeof( VertexType) * (sentence->vertexCount ) ) );

	float drawX = (float)( ( ( m_ScreenWidth / 2 ) * ( -1 ) + positionX ) );
	float drawY = (float)( ( ( m_ScreenHeight / 2 ) - positionY ) );

	m_Font->BuildVertexArray( (void*)vertices, text, drawX, drawY );

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory( &mappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );

	hr = DevContext->Map( sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Close Vertex Buffer \n ");
		return false;
	}

	VertexType* verticesPtr = (VertexType*)mappedResource.pData;

	memcpy( verticesPtr, (void*)vertices, ( sizeof(VertexType) * sentence->vertexCount ) );

	DevContext->Unmap( sentence->vertexBuffer, 0 );

	delete[] vertices;
	vertices = nullptr;

	return true;
}


bool DXTEXT::RenderSentence( ID3D11DeviceContext* DevContext, SentenceType* sentence, XMMATRIX worldMatrix, XMMATRIX  orthoMatrix )
{
	UINT stride = sizeof( VertexType );
	UINT offset = 0;

	DevContext->IASetVertexBuffers( 0, 1, &sentence->vertexBuffer, &stride, &offset );
	DevContext->IASetIndexBuffer( sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	DevContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	XMFLOAT4 pixelColor = XMFLOAT4( sentence->Red, sentence->Green, sentence->Blue, 1.0f );

	if ( !m_FontShader->Render( DevContext, sentence->indexCount, worldMatrix, m_BaseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor ) )
	{
		LOG_ERROR(" Failed - Shade for Font \n ");
		return false;
	}

	return true;
}


void DXTEXT::InitPointer()
{
	m_Font = nullptr;
	m_FontShader = nullptr;

	m_Sentence1 = nullptr;
	m_Sentence2 = nullptr;
}

