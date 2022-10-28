#ifndef __DXTEXTURE_H__
#define __DXTEXTURE_H__

#include "DXCommon.h"

class DXTEXTURE
{
	public :

		DXTEXTURE();
		DXTEXTURE( const DXTEXTURE* );
		~DXTEXTURE();

		void Release();

		ID3D11ShaderResourceView* GetTexture();

		bool LoadTexture( ID3D11Device*, const char* );

	private :

		void InitPointer();

	private :

		ID3D11ShaderResourceView* m_TextureView;
};

#endif