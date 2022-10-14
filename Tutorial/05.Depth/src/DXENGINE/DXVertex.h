#ifndef __DXVERTEX_H__
#define __DXVERTEX_H__

#include "DXCommon.h"

struct Vertex
{
	Vertex(){}
	Vertex( float x, float y, float z,
		float cr, float cg, float cb, float ca ) : pos( x, y, z ), color( cr, cg, cb, ca){}

	XMFLOAT3 pos;
	XMFLOAT4 color;
};

#endif