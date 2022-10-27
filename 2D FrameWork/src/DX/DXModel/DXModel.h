#ifndef __DXMODEL_H__
#define __DXMODEL_H__

#include "DXCommon.h"

class DXMODEL
{
	struct VertexType
	{
		XMFLOAT3 POS;
		XMFLOAT4 COLOR;
		XMFLOAT3 NORMAL;
	};

	struct ModelType
	{
		float X, Y, Z;
		float NX, NY, NZ;
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
		bool Update( ID3D11DeviceContext* );

	// Functions - Divide Init Function
	private :

		bool SetVertex();
		bool SetIndex();

		bool InitVertexBuffer( ID3D11Device* );
		bool InitIndexBuffer( ID3D11Device* );

		bool UpdateVertexBuffer( ID3D11DeviceContext* );
		bool UpdateIndexBuffer( ID3D11DeviceContext* );

		void InitPointer();

		bool LoadModel();

	// Functions - Get
	public :

		int GetIndexCount();

	// Variables
	private :

		ID3D11Buffer* m_VertexBuffer;
		ID3D11Buffer* m_IndexBuffer;
		int m_VertexCount;
		int m_IndexCount;
		VertexType* m_Vertices;
		UINT* m_Indices;

		float Red;
		float Green;
		float Blue;

		float temp_R;
		float temp_G;
		float temp_B;

		ModelType* m_Model;
		const char* m_ModelFile;
};

#endif