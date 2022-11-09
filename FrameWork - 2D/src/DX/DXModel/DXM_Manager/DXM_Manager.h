#ifndef __DXM_MANAGER_H__
#define __DXM_MANAGER_H__

#include "DXCommon.h"
#include "DXPhysics.h"

typedef enum DXMPOLYGON
{
	DXMTRIANGLE = 3,
	DXMRECTANGLE = 4,
	DXMPENTAGON = 5,
	DXMHEXAGON = 6,
	DXMCIRCLE = 30,
}DXMPOLYGON;

struct VERTEXINFO
{
	XMFLOAT3 POS;
	XMFLOAT4 COLOR;
	XMFLOAT3 NORMAL;
};

struct MODELINFO
{
	XMFLOAT3 POS;
	XMFLOAT3 ROT;
};

struct TYPEINFO
{
	DXMPOLYGON TYPE;

	int NumModel;
	MODELINFO* MODELS;

	int NumVertex;
	VERTEXINFO* VERTICES;

	int NumIndex;
	UINT* INDICES;
};

class DXM_MANAGER
{
	public :

		DXM_MANAGER();
		DXM_MANAGER( const DXM_MANAGER* );
		~DXM_MANAGER();

	public :

		bool Init( TYPEINFO*&, const int, DXMPOLYGON );
		void Release( TYPEINFO*& );
		bool Frame( TYPEINFO*&, bool, int, int );

	private :

		void Rotation( float, float, float, MODELINFO*& );
		void Translation( float, float, float, MODELINFO*& );
		void Scale( float, float ,float );

		void InitPointer();

		void Destory( const int );
		void Create( TYPEINFO*& );
		void Insert( TYPEINFO*&, int, int );
};

#endif