#ifndef __MDTEXTURE_H__
#define __MDTEXTURE_H__

#include "DXCommon.h"

class MDTEXTURE
{
	private :

		struct TextHeadr
		{
			unsigned char data1[ 12 ];
			unsigned short width;
			unsigned short height;
			unsigned char bpp;
			unsigned char data2;
		};

	public :

		MDTEXTURE();
		MDTEXTURE( const MDTEXTURE* );
		~MDTEXTURE();

		bool Init( ID3D11Device*, ID3D11DeviceContext*, char* );
		void Release();

		ID3D11ShaderResourceView* GetTexture();

	private :

		void InitPointer();
		bool LoadTexture( char*, int&, int& );

	private :

		unsigned char* m_TextureData;
		ID3D11Texture2D* m_Texture;
		ID3D11ShaderResourceView* m_TextureView;
};

#endif