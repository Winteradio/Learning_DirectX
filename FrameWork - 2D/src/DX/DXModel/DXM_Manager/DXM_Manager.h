#ifndef __DXM_MANAGER_H__
#define __DXM_MANAGER_H__

#include "DXCommon.h"

typedef enum DXMPOLYGON
{
	DXMTRIANGLE = 3,
	DXMRECTANGLE = 4,
	DXMPENTAGON = 5,
	DXMHEXAGON = 6,
	DXMCIRCLE = 50,
}DXMPOLYGON;

struct VERTEXINFO
{
	XMFLOAT3 POS;
	XMFLOAT4 COLOR;
	XMFLOAT3 NORMAL;
};

struct MODELINFO
{
	DXMPOLYGON TYPE;
	XMFLOAT3 POS;
	XMFLOAT3 ROT;
	VERTEXINFO* VERTICES;
	UINT* INDICES;
	int NumVertex;
	int NumIndex;
};

struct TYPEINFO
{
	DXMPOLYGON TYPE;
	int NumModel;
	int NumVertex;
	int NumIndex;
	MODELINFO* MODELS;
};

class DXM_MANAGER
{
	public :

		DXM_MANAGER();
		DXM_MANAGER( const DXM_MANAGER* );
		~DXM_MANAGER();

	public :

		bool Init( MODELINFO*&, const int, DXMPOLYGON );
		void Release( MODELINFO*& );
		bool Frame();

	private :

		void Rotation( float, float, float, MODELINFO& );
		void Translation( float, float, float, MODELINFO& );
		void Scale( float, float ,float );

		void InitPointer();

		void Destory( const int );
		void Create( MODELINFO&, DXMPOLYGON );
};

#endif