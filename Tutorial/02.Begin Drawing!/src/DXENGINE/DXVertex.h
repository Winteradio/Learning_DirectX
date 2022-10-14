#pragma once

#ifndef __DXVERTEX_H__
#define __DXVERTEX_H__

#include "DXCommon.h"

struct Vertex
{
	Vertex(){}
	Vertex( float x, float y, float z) : pos( x, y, z ){}

	XMFLOAT3 pos;
};

#endif