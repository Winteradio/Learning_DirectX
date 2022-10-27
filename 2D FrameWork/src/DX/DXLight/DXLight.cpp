#include "DXLight.h"

DXLIGHT::DXLIGHT(){}

DXLIGHT::DXLIGHT( const DXLIGHT* Other )
{
	*this = *Other;
}

DXLIGHT::~DXLIGHT(){}

void DXLIGHT::SetAmbientColor( float X, float Y, float Z, float W )
{
	m_AmbientColor = XMFLOAT4( X, Y, Z, W );
}

void DXLIGHT::SetDiffuseColor( float X, float Y, float Z, float W )
{
	m_DiffuseColor = XMFLOAT4( X, Y, Z ,W );
}

void DXLIGHT::SetDirection( float X, float Y, float Z )
{
	m_Direction = XMFLOAT3( X, Y ,Z );
}

void DXLIGHT::SetSpecularColor( float X, float Y, float Z, float W )
{
	m_SpecularColor = XMFLOAT4( X, Y ,Z ,W );
}

void DXLIGHT::SetSpecularPower( float X )
{
	m_SpecularPower = X;
}

XMFLOAT4 DXLIGHT::GetAmbientColor() { return m_AmbientColor; }

XMFLOAT4 DXLIGHT::GetDiffuseColor() { return m_DiffuseColor; }

XMFLOAT3 DXLIGHT::GetDirection() { return m_Direction; }

XMFLOAT4 DXLIGHT::GetSpecularColor() { return m_SpecularColor; }

float DXLIGHT::GetSpecularPower() { return m_SpecularPower; }