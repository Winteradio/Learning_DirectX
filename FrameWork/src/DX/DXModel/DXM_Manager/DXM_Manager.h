#ifndef __DXM_MANAGER_H__
#define __DXM_MANAGER_H__

#include "DXCommon.h"

struct VertexType
{
	XMFLOAT3 POS;
	XMFLOAT2 TEXTURE;
	XMFLOAT3 NORMAL;
};

struct PlaneType
{
	XMFLOAT3* POINTS;
	XMFLOAT3 NORMAL;
};

class DXM_MANAGER
{
	public :

		struct ModelInfo
		{
			int NumPlane;
			PlaneType* Planes;
			XMFLOAT3 POS;
			XMFLOAT3 ROT;
		};

	public :

		DXM_MANAGER();
		DXM_MANAGER( const DXM_MANAGER* );
		~DXM_MANAGER();

	public :

		bool Init( const VertexType*, const UINT*, const int, const int );
		void Release();
		bool Frame();

	private :

		void Rotation( float, float, float );
		void Translation( float, float, float );
		void Scale( float, float ,float );

		void InitPointer();

		void Destory( const int );
		void Create( const VertexType*, const UINT*, const int );
};

#endif