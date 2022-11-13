#ifndef __DXM_COMMON_H__
#define __DXM_COMMON_H__

#include "DXCommon.h"

typedef enum DXMPOLYGON
{
	DXMTRIANGLE = 3,
	DXMRECTANGLE = 4,
	DXMPENTAGON = 5,
	DXMHEXAGON = 6,
	DXMCIRCLE = 10,
}DXMPOLYGON;

struct VERTEXINFO
{
	XMFLOAT3 POS  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT4 COLOR  = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
	XMFLOAT3 NORMAL = XMFLOAT3( 0.0f, 0.0f, 0.0f );
};

struct MODELINFO
{
	XMFLOAT3 ACC = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 VEL = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 POS = XMFLOAT3( 0.0f, 0.0f, 0.0f );

	XMFLOAT3 ANGACC = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 ANGVEL = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 ANG = XMFLOAT3( 0.0f, 0.0f, 0.0f );

	float MASS = 0.0f;
};

struct TYPEINFO
{
	DXMPOLYGON TYPE;

	int NumModel = 0;
	int Length = 0;
	MODELINFO* MODELS = nullptr;

	int NumVertex = 0;
	VERTEXINFO* VERTICES = nullptr;

	int NumIndex = 0;
	UINT* INDICES = nullptr;
};

#endif