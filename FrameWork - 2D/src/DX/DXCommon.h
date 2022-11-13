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

inline float DXDOT( XMFLOAT2 Num1, XMFLOAT2 Num2 ) { return Num1.x * Num2.x + Num1.y * Num2.y; }
inline float DXDOT( XMFLOAT3 Num1, XMFLOAT3 Num2 ) { return Num1.x * Num2.x + Num1.y * Num2.y + Num1.z * Num2.z; }
inline float DXDOT( XMFLOAT4 Num1, XMFLOAT4 Num2 ) { return Num1.x * Num2.x + Num1.y * Num2.y + Num1.z * Num2.z + Num1.w * Num2.w; }

inline XMFLOAT3 DXCROSS( XMFLOAT3 Num1, XMFLOAT3 Num2 )
{ return XMFLOAT3(
	Num1.y * Num2.z - Num1.z * Num2.y,
	Num1.z * Num2.x - Num1.x * Num2.z,
	Num1.x * Num2.y - Num1.y * Num2.x );}

inline XMFLOAT3 DXADD( XMFLOAT3 Num1, XMFLOAT3 Num2 ) { return XMFLOAT3( Num1.x + Num2.x, Num1.y + Num2.y, Num1.z + Num2.z ); }
inline XMFLOAT3 DXSUBTRACT( XMFLOAT3 Num1, XMFLOAT3 Num2 ) { return XMFLOAT3( Num1.x - Num2.x, Num1.y - Num2.y, Num1.z - Num2.z ); }
inline XMFLOAT3 DXMULTIPLY( XMFLOAT3 Num, float Value ) { return XMFLOAT3( Num.x * Value, Num.y * Value, Num.z * Value); }
inline XMFLOAT3 DXDIVIDE( XMFLOAT3 Num, float Value ) { return XMFLOAT3( Num.x / Value, Num.y / Value, Num.z / Value); }

inline XMFLOAT3 DXUNIT( XMFLOAT3 Num )
{
	if ( DXDOT( Num, Num ) > 0.0f ) { return DXDIVIDE( Num, sqrt( DXDOT( Num, Num ) ) ); }
	else { return XMFLOAT3( 0.0f, 0.0f, 0.0f ); }
}

#endif