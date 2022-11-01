#ifndef __DXT_FONT_H__
#define __DXT_FONT_H__

#include "DXCommon.h"
#include "DXTexture.h"

class DXT_FONT
{
	private :

		struct FontType
		{
			float left;
			float right;
			float size;
		};

		struct VertexType
		{
			XMFLOAT3 POS;
			XMFLOAT2 TEXTURE;
		};

	public :

		DXT_FONT();
		DXT_FONT( const DXT_FONT* );
		~DXT_FONT();

	public :

		bool Init( ID3D11Device*, const char*, const char* );
		void Release();

		ID3D11ShaderResourceView* GetTexture();

		void BuildVertexArray( void*, const char*, float, float );

	private :

		bool LoadFontData( const char* );
		bool LoadTexture( ID3D11Device*, const char* );

		void InitPointer();

	private :

		FontType* m_Font;
		DXTEXTURE* m_Texture;
};

#endif