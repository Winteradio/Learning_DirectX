#ifndef __DXMODEL_H__
#define __DXMODEL_H__

#include "DXCommon.h"
#include "MDTexture.h"

class DXMODEL
{
	struct VertexType
	{
		XMFLOAT3 POS;
		XMFLOAT2 TEXTURE;
	};

	// Functions - Constructor and Destructor
	public :

		DXMODEL();
		DXMODEL( const DXMODEL* );
		~DXMODEL();

	// Functions - Init, Release Object and Render
	public :

		bool Init( ID3D11Device*, int ,int , char*, int ,int );
		void Release();
		void Render( ID3D11DeviceContext*, int );
		bool Update( ID3D11DeviceContext*, int );

	// Functions - Divide Init Function
	private :

		bool SetVertex();
		bool SetIndex();

		bool InitVertexBuffer( ID3D11Device* );
		bool InitIndexBuffer( ID3D11Device* );

		bool UpdateVertexBuffer( ID3D11DeviceContext* );
		bool UpdateIndexBuffer( ID3D11DeviceContext* );

		void InitPointer();

		bool LoadTexture( ID3D11Device*, char* );

	// Functions - Get
	public :

		int GetIndexCount();
		ID3D11ShaderResourceView* GetTexture();

	// Variable
	private :

		ID3D11Buffer* m_VertexBuffer;
		ID3D11Buffer* m_IndexBuffer;

		int m_VertexCount;
		int m_IndexCount;

		MDTEXTURE* m_Texture;

		int m_ScreenWidth;
		int m_ScreenHeight;
		int m_BitmapWidth;
		int m_BitmapHeight;
		int m_PreviousPosX;
		int m_PreviousPosY;
};

#endif