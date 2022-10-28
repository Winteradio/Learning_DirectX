#ifndef __DXTEXT_H__
#define __DXTEXT_H__

#include "DXCommon.h"
#include "DXT_Font.h"
#include "DXT_Shader.h"

class DXTEXT
{
	private :

		struct SentenceType
		{
			ID3D11Buffer* vertexBuffer;
			ID3D11Buffer* indexBuffer;
			int vertexCount;
			int indexCount;
			int maxLength;
			float Red;
			float Green;
			float Blue;
		};

		struct VertexType
		{
			XMFLOAT3 POS;
			XMFLOAT2 TEXTURE;
		};

	public :

		DXTEXT();
		DXTEXT( const DXTEXT* );
		~DXTEXT();

	public :

		bool Init( ID3D11Device*, ID3D11DeviceContext*,
			int, int, XMMATRIX,
			const char*, const char*,
			const char*, const char*);
		void Release();
		bool Render( ID3D11DeviceContext*, XMMATRIX, XMMATRIX );

	private :

		bool InitFont( ID3D11Device*, const char*, const char* );
		bool InitFontShader( ID3D11Device*, ID3D11DeviceContext*, const char*, const char* );

		bool InitSentence( SentenceType**, int, ID3D11Device* );
		bool UpdateSentence( SentenceType*, char*, int, int, float, float ,float, ID3D11DeviceContext* );
		void ReleaseSentence( SentenceType** );
		bool RenderSentence( ID3D11DeviceContext*, SentenceType*, XMMATRIX, XMMATRIX );

		void InitPointer();


	private :

		DXT_FONT* m_Font;
		DXT_SHADER* m_FontShader;

		int m_ScreenWidth;
		int m_ScreenHeight;

		XMMATRIX m_BaseViewMatrix;
		SentenceType* m_Sentence1;
		SentenceType* m_Sentence2;
};

#endif