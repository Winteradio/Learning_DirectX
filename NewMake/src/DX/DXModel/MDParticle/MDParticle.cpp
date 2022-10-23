#include "MDParticle.h"

MDPARTICLE::MDPARTICLE()
{
	Init();
}

MDPARTICLE::MDPARTICLE( const MDPARTICLE* Other )
{
	*this = *Other;
}

MDPARTICLE::MDPARTICLE( XMFLOAT3 Pos, XMFLOAT4 Color )
{
	m_POSITION = Pos;
	m_COLOR = Color;
}

MDPARTICLE::~MDPARTICLE(){}

void MDPARTICLE::Init()
{
	m_Connection = {};
	SetMass( 0.0f );
	SetForce( 0.0f, 0.0f, 0.0f );
	SetVelocity( 0.0f, 0.0f, 0.0f );
	SetPosition( 0.0f, 0.0f, 0.0f );
	SetColor( 0.0f, 0.0f, 0.0f, 0.0f );
}

void MDPARTICLE::Link( const MDPARTICLE* Other )
{
}

void MDPARTICLE::SetMass( float Mass )
{
	m_MASS = Mass;
}

void MDPARTICLE::SetForce( float X, float Y, float Z )
{
	m_FORCE.x = X;
	m_FORCE.y = Y;
	m_FORCE.z = Z;
}

void MDPARTICLE::SetVelocity( float X, float Y, float Z)
{
	m_VELOCITY.x = X;
	m_VELOCITY.y = Y;
	m_VELOCITY.z = Z;
}

void MDPARTICLE::SetPosition( float X, float Y, float Z )
{
	m_POSITION.x = X;
	m_POSITION.y = Y;
	m_POSITION.z = Z;
}

void MDPARTICLE::SetColor( float R, float G, float B, float A )
{
	m_COLOR.x = R;
	m_COLOR.y = G;
	m_COLOR.z = B;
	m_COLOR.w = A;
}

float MDPARTICLE::GetMass() { return m_MASS; }

XMFLOAT3 MDPARTICLE::GetForce() { return m_FORCE; }

XMFLOAT3 MDPARTICLE::GetVelocity() { return m_VELOCITY; }

XMFLOAT3 MDPARTICLE::GetPosition() { return m_POSITION; }

XMFLOAT4 MDPARTICLE::GetColor() { return m_COLOR; }
