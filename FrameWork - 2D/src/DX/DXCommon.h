#pragma warning ( disable : 4838 )

#ifndef __DXCOMMON_H__
#define __DXCOMMON_H__

#define _USE_MATH_DEFINES

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include <xnamath.h>
#include <cmath>

#include "SUBLog.h"

inline bool DXCOMP( XMFLOAT2 Num1, XMFLOAT2 Num2 ) { return Num1.x == Num2.x && Num1.y == Num2.y; }
inline bool DXCOMP( XMFLOAT3 Num1, XMFLOAT3 Num2 ) { return Num1.x == Num2.x && Num1.y == Num2.y && Num1.z == Num2.z; }
inline bool DXCOMP( XMFLOAT4 Num1, XMFLOAT4 Num2 ) { return Num1.x == Num2.x && Num1.y == Num2.y && Num1.z == Num2.z && Num1.w == Num2.w; }

#endif