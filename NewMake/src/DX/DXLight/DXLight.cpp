#include "DXLight.h"

DXLIGHT::DXLIGHT(){}

DXLIGHT::DXLIGHT( const DXLIGHT* Other )
{
	*this = *Other;
}

DXLIGHT::~DXLIGHT(){}

void DXLIGHT::SetAmbientColor( float X, float Y, float Z, float W )
{
	m_AmbientColor.x = X;
	m_AmbientColor.y = Y;
	m_AmbientColor.z = Z;
	m_AmbientColor.w = W;
}

void DXLIGHT::SetDiffuseColor( float X, float Y, float Z, float W )
{
	m_DiffuseColor.x = X;
	m_DiffuseColor.y = Y;
	m_DiffuseColor.z = Z;
	m_DiffuseColor.w = W;
}

void DXLIGHT::SetDirection( float X, float Y, float Z )
{
	m_Direction.x = X;
	m_Direction.y = Y;
	m_Direction.z = Z;
}

XMFLOAT4 DXLIGHT::GetAmbientColor() { return m_AmbientColor; }

XMFLOAT4 DXLIGHT::GetDiffuseColor() { return m_DiffuseColor; }

XMFLOAT3 DXLIGHT::GetDirection() { return m_Direction; }