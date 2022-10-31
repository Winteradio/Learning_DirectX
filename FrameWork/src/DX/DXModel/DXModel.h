#ifndef __DXMODEL_H__
#define __DXMODEL_H__

#include "DXCommon.h"
#include "DXTexture.h"
#include "DXM_Manager.h"

class DXMODEL
{
	public :

		struct ModelTXT
		{
			float X, Y, Z;
			float TU, TV;
			float NX, NY, NZ;
		};

	// Functions - Constructor and Destructor
	public :

		DXMODEL();
		DXMODEL( const DXMODEL* );
		~DXMODEL();

	// Functions - Init, Release Object and Render
	public :

		bool Init( ID3D11Device*, const char*, const char* );
		void Release();
		void Render( ID3D11DeviceContext* );
		bool Update( ID3D11DeviceContext* );

	// Functions - Divide Init Function
	private :

		bool SetVertex();
		bool SetIndex();

		bool InitVertexBuffer( ID3D11Device* );
		bool InitIndexBuffer( ID3D11Device* );
		bool InitOutputBuffer( ID3D11Device* );
		bool InitDXMMANGER( VertexType*, UINT*, int );

		bool UpdateVertexBuffer( ID3D11DeviceContext* );
		bool UpdateIndexBuffer( ID3D11DeviceContext* );
		bool UpdateOutputBuffer( ID3D11DeviceContext* );

		void InitPointer();

		bool LoadTexture( ID3D11Device*, const char* );
		bool LoadModel( const char* );

	// Functions - Get
	public :

		int GetIndexCount();
		ID3D11ShaderResourceView* GetTexture();

	// Variables
	private :

		ID3D11Buffer* m_VertexBuffer;
		ID3D11Buffer* m_IndexBuffer;
		ID3D11Buffer* m_OutputBuffer;

		int m_VertexCount;
		int m_IndexCount;
		int m_ModelCount;

		VertexType* m_Vertices;
		UINT* m_Indices;

		ModelTXT* m_ModelTXT;

		DXTEXTURE* m_DXTEXTURE;
		DXM_MANAGER* m_DXMMANGER;
};

#endif