#ifndef __DXMODEL_H__
#define __DXMODEL_H__

#include "DXCommon.h"
#include "DXTexture.h"
#include "DXM_Manager.h"
#include "DXM_Physics.h"

class DXMODEL
{
	// Functions - Constructor and Destructor
	public :

		DXMODEL();
		DXMODEL( const DXMODEL* );
		~DXMODEL();

	// Functions - Init, Release Object and Render
	public :

		bool Init( int, int, ID3D11Device*, const char*, const char* );
		void Release();
		void Render( ID3D11DeviceContext* );
		bool Update( ID3D11DeviceContext* );
		bool Frame( bool, int, int, float, float );

	// Functions - Divide Init Function
	private :

		bool InitDXMMANGER( DXMPOLYGON );
		bool InitDXMPHYSICS( int, int, float, float, float, float, float );
		bool InitVertexBuffer( ID3D11Device*, int );
		bool InitIndexBuffer( ID3D11Device*, int );
		void InitPointer();

		bool LoadTexture( ID3D11Device*, const char* );

	// Functions - Get
	public :

		int GetIndexCount( int );
		int GetVertexCount( int );
		int GetNumModel( int );
		XMFLOAT3 GetPosition( int, int  );
		ID3D11ShaderResourceView* GetTexture();
		TYPEINFO* m_DXMODELLIST;

	// Variables
	private :

		ID3D11Buffer* m_VertexBuffer;
		ID3D11Buffer* m_IndexBuffer;

		ModelTXT* m_ModelTXT;

		DXTEXTURE* m_DXTEXTURE;
		DXM_MANAGER* m_DXMMANGER;
		DXM_PHYSICS* m_DXMPHYSICS;
};

#endif