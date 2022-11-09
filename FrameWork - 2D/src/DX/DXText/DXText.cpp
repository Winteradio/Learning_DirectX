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


bool DXTEXT::Init( ID3D11Device* Device, ID3D11DeviceContext* DevContext,
	int screenWidth, int screenHeight, XMMATRIX baseViewMatrix,
	const char* FontfileDIR, const char* TexfileDIR, const char* VSfileDIR, const char* PSfileDIR )
{
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	m_BaseViewMatrix = baseViewMatrix;

	m_SenCount = 5;
	m_MaxText = 16;

	if ( !InitFont( Device, FontfileDIR, TexfileDIR ) ) { return false; }
	if ( !InitFontShader( Device, DevContext, VSfileDIR, PSfileDIR ) ) { return false; }
	if ( !InitSentence( Device, m_MaxText ) ) { return false; }

	return true;
}


void DXTEXT::Release()
{
	m_Font->Release();
	m_FontShader->Release();

	for ( int I = 0; I < m_SenCount; I++ )
	{
		ReleaseSentence( m_SentenceList[ I ] );
	}

	delete[] m_SentenceList;

	InitPointer();
}


void DXTEXT::ReleaseSentence( SentenceType*& sentence )
{
	sentence->vertexBuffer->Release();
	sentence->indexBuffer->Release();
	sentence = nullptr;
}


bool DXTEXT::Render( ID3D11DeviceContext* DevContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix )
{
	for ( int I = 0; I < m_SenCount; I++ )
	{
		if ( !RenderSentence( DevContext, m_SentenceList[ I ], worldMatrix, orthoMatrix ) ) { return false; }
	}

	return true;
}


bool DXTEXT::Frame( ID3D11DeviceContext* DevContext, int mouseX, int mouseY, int CPU, int FPS, int numModel )
{
	XMFLOAT4 Color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	if ( !SetSentence( DevContext, m_SentenceList[0], "FPS : ", FPS, 20, 30, Color ) ) { return false; }
	if ( !SetSentence( DevContext, m_SentenceList[1], "CPU : ", CPU, 20, 60, Color ) ) { return false; }
	if ( !SetSentence( DevContext, m_SentenceList[2], "Mouse X : ", mouseX, 200, 30, Color ) ) { return false; }
	if ( !SetSentence( DevContext, m_SentenceList[3], "Mouse Y : ", mouseY, 200 ,60, Color ) ) { return false; }
	if ( !SetSentence( DevContext, m_SentenceList[4], "Num : ", numModel, 20 ,90, Color ) ) { return false; }

	return true;
}


bool DXTEXT::SetSentence( ID3D11DeviceContext* DevContext, SentenceType*& sentence, char* Text, int Info, int posX, int posY, XMFLOAT4 color )
{
	char tempString[ 16 ] = {0,};
	_itoa_s( Info, tempString, 10 );

	char resultString[ 16 ] = {0,};
	strcpy_s( resultString, Text );
	strcat_s( resultString, tempString );

	if ( !UpdateSentence( DevContext, sentence, resultString, posX, posY, color.x, color.y, color.z ) )
	{
		LOG_ERROR(" Failed - Update %s \n ", Text );
		return false;
	}

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


bool DXTEXT::InitSentence( ID3D11Device* Device, int maxLength )
{
	m_SentenceList = new SentenceType*[ m_SenCount ];
	if ( !m_SentenceList )
	{
		LOG_ERROR(" Failed - Create Sentence Pointer List \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Sentence Pointer List \n ");
	}

	for ( int I = 0; I < m_SenCount; I++ )
	{
		m_SentenceList[ I ] = new SentenceType;

		if ( !m_SentenceList[ I ] )
		{
			LOG_ERROR(" Failed - Create Sentence \n ");
			return false;
		}

		m_SentenceList[ I ]->vertexBuffer = nullptr;
		m_SentenceList[ I ]->indexBuffer = nullptr;
		m_SentenceList[ I ]->vertexCount = 6 * maxLength;
		m_SentenceList[ I ]->indexCount = 6 * maxLength;
		m_SentenceList[ I ]->maxLength = maxLength;
		m_SentenceList[ I ]->Red = 0.0f;
		m_SentenceList[ I ]->Green = 0.0f;
		m_SentenceList[ I ]->Blue = 0.0f;

		if ( !InitVertexBuffer( Device, m_SentenceList[ I ] ) ) { return false; }
		if ( !InitIndexBuffer( Device, m_SentenceList[ I ] ) ) { return false; }
	}

	return true;
}


bool DXTEXT::UpdateSentence( ID3D11DeviceContext* DevContext, SentenceType*& sentence, char* text,
	int positionX, int positionY,
	float red, float blue ,float green )
{
	HRESULT hr;

	sentence->Red = red;
	sentence->Blue = blue;
	sentence->Green = green;

	// Check text length that is over maximum
	int numLetters = (int)strlen(text);
	if ( numLetters > sentence->maxLength )
	{
		LOG_ERROR(" Over the maxLength : %d that has Numbers of letters %d \n ", sentence->maxLength, numLetters );
		return false;
	}

	// Update vertex
	VertexType* vertices = new VertexType[ sentence->vertexCount ];
	if ( !vertices )
	{
		LOG_ERROR(" Failed - Create Vertex for Sentence \n ");
		return false;
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


bool DXTEXT::InitVertexBuffer( ID3D11Device* Device, SentenceType*& sentence )
{
	HRESULT hr;

	VertexType* vertices = new VertexType[ sentence->vertexCount ];

	if ( !vertices )
	{
		LOG_ERROR(" Failed - Create Vertex for sentence\n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Vertex for sentence\n ");
	}

	memset( vertices, 0, ( sizeof( VertexType ) * sentence->vertexCount ) );

	// Describe vertex buffer;
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof( VertexType ) * sentence->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Describe vertex data
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory( &vertexData, sizeof( D3D11_SUBRESOURCE_DATA ) );

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer on sentence
	hr = Device->CreateBuffer( &vertexBufferDesc, &vertexData, &sentence->vertexBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Vertex Buffer \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Vertex Buffer \n ");
	}

	delete[] vertices;
	vertices = nullptr;

	return true;
}


bool DXTEXT::InitIndexBuffer( ID3D11Device* Device, SentenceType*& sentence )
{
	HRESULT hr;

	UINT* indices = new UINT[ sentence->indexCount ];

	if ( !indices )
	{
		LOG_ERROR(" Failed - Create Index for sentence \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Index for sentence \n ");
	}

	for ( int J = 0; J < sentence->indexCount; J++ )
	{
		indices[ J ] = J;
	}

	// Describe index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof( UINT ) * sentence->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Describe index Data
	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory( &indexData, sizeof( D3D11_SUBRESOURCE_DATA ) );

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer on sentence
	hr = Device->CreateBuffer( &indexBufferDesc, &indexData, &sentence->indexBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Index Buffer \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Index Buffer \n ");
	}

	delete[] indices;
	indices = nullptr;

	return true;
}


void DXTEXT::InitPointer()
{
	m_Font = nullptr;
	m_FontShader = nullptr;

	m_SentenceList = nullptr;
}

