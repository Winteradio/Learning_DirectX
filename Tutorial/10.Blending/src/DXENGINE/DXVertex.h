#pragma once

#ifndef __DXVERTEX_H__
#define __DXVERTEX_H__

#include "DXCommon.h"

struct Vertex
{
	Vertex(){}
	Vertex( float x, float y, float z,
		float u, float v ) : pos( x, y, z ), texCoord( u, v ){}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
};

struct cbPerObject
{
	XMMATRIX WVP;
};

#endif