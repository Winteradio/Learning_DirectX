#ifndef __MDTEXTURE_H__
#define __MDTEXTURE_H__

#include "DXCommon.h"

class MDTEXTURE
{
	public :

		MDTEXTURE();
		MDTEXTURE( const MDTEXTURE* );
		~MDTEXTURE();

		bool Init( ID3D11Device* );
		void Release();

		ID3D11ShaderResourceView* GetTexture();

	private :

		void InitPointer();
		bool LoadTexture( ID3D11Device* );

	private :

		const char* m_TextureFile;
		ID3D11ShaderResourceView* m_TextureView;
};

#endif