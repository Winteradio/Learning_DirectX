#ifndef __DXMODEL_H__
#define __DXMODEL_H__

#include "DXCommon.h"

#define COORDINATE_1 100
#define COORDINATE_2 200
#define COORDINATE_3 300

class DXMODEL
{
	struct VertexType
	{
		XMFLOAT3 POS;
		XMFLOAT4 COLOR;
	};

	// Functions - Constructor and Destructor
	public :

		DXMODEL();
		DXMODEL( const DXMODEL* );
		~DXMODEL();

	// Functions - Init, Release Object and Render
	public :

		bool Init( ID3D11Device* );
		void Release();
		void Render( ID3D11DeviceContext* );

	// Functions - Divide Init Function
	private :

		bool InitVertex( ID3D11Device* );
		bool InitIndex( ID3D11Device* );
		void InitPointer();

	// Functions - Get
	public :

		int GetIndexCount();

	// Variables
	private :

		ID3D11Buffer* m_VertexBuffer;
		ID3D11Buffer* m_IndexBuffer;
		int m_VertexCount;
		int m_IndexCount;
};

#endif