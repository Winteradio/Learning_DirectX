#include "DXM_Physics.h"

DXM_PHYSICS::DXM_PHYSICS() {}

DXM_PHYSICS::DXM_PHYSICS( const DXM_PHYSICS* Other )
{
	*this = *Other;
}

DXM_PHYSICS::~DXM_PHYSICS() {}

bool DXM_PHYSICS::Init( int Width, int Height, float gravity, float spring, float demper, float drag, float friction )
{
	m_GravityConstant = gravity;
	m_SpringConstant = spring;
	m_DemperConstant = demper;
	m_DragConstant = drag;
	m_FrictionConstant = friction;

	m_NumFence = 3;
	m_Fence = new FENCE[ m_NumFence ];
	// Bottom
	m_Fence[0].Range = 10.0f;
	m_Fence[0].Outer = -(float)Height/2;
	m_Fence[0].NOR = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	m_Fence[0].DOT = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	// Left
	m_Fence[1].Range = 10.0f;
	m_Fence[1].Outer = -(float)Width/2;
	m_Fence[1].NOR = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	m_Fence[1].DOT = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	// Right
	m_Fence[2].Range = 10.0f;
	m_Fence[2].Outer = (float)Width/2;
	m_Fence[2].NOR = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	m_Fence[2].DOT = XMFLOAT3( 1.0f, 0.0f, 0.0f );

	m_ERROR = 0.01f;

	return true;
}

bool DXM_PHYSICS::Frame( MODELINFO*& modelList, int numModel, float timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		m_Collision = false;
		m_Contect = false;
		if ( !CalEulerMethod( modelList[ I ], timeStep ) ) { return false ; }
	}

	return true;
}

void DXM_PHYSICS::Release()
{
	m_Fence = nullptr;
}


bool DXM_PHYSICS::CalEulerMethod( MODELINFO& model, float timeStep )
{
	if ( !CalAccelerate( model, timeStep ) ) { return false; }
	if ( !CalVelocity( model, timeStep ) ) { return false; }

	if ( !CalAngAccelerate( model, timeStep ) ) { return false; }
	if ( !CalAngVelocity( model, timeStep ) ) { return false; }

	if ( !CalPosition( model, timeStep ) ) { return false; }
	if ( !CalAngle( model, timeStep ) ) { return false; }

	return true;
}


bool DXM_PHYSICS::CalAccelerate( MODELINFO& model, float timeStep )
{
	InitForce( model.ACC );
	//CalCollision( model, timeStep );
	CalContect( model, timeStep );
	SetGravityForce( model.ACC, timeStep );
	SetDragForce( model.ACC, model.VEL, timeStep );
	return true;
}

bool DXM_PHYSICS::CalVelocity( MODELINFO& model, float timeStep )
{
	model.VEL = XMFLOAT3( model.VEL.x + model.ACC.x * timeStep,
		model.VEL.y + model.ACC.y * timeStep,
		model.VEL.z + model.ACC.z * timeStep );
	return true;
}

bool DXM_PHYSICS::CalPosition( MODELINFO& model, float timeStep )
{
	model.POS = XMFLOAT3( model.POS.x + model.VEL.x * timeStep,
		model.POS.y + model.VEL.y * timeStep,
		model.POS.z + model.VEL.z * timeStep );
	return true;
}

bool DXM_PHYSICS::CalAngAccelerate( MODELINFO& model, float timeStep )
{
	model.ANGACC = XMFLOAT3( model.ANGACC.x, model.ANGACC.y, model.ACC.z );
	return true;
}

bool DXM_PHYSICS::CalAngVelocity( MODELINFO& model, float timeStep )
{
	model.ANGVEL = XMFLOAT3( model.ANGVEL.x + model.ANGACC.x * timeStep,
		model.ANGVEL.y + model.ANGACC.y * timeStep,
		model.ANGVEL.z + model.ANGACC.z * timeStep );
	return true;
}

bool DXM_PHYSICS::CalAngle( MODELINFO& model, float timeStep )
{
	model.ANG = XMFLOAT3( model.ANG.x + model.ANGVEL.x * timeStep,
		model.ANG.y + model.ANGVEL.y * timeStep,
		model.ANG.z + model.ANGVEL.z * timeStep );
	return true;
}

bool DXM_PHYSICS::CalCollision( MODELINFO& model, float timeStep )
{
	for (int I = 0; I < m_NumFence; I++ )
	{
		float posDOT = DXDOT( model.POS, m_Fence[ I ].DOT );
		float velDOT = DXDOT( model.VEL, m_Fence[ I ].NOR );
	}
	return true;
}

bool DXM_PHYSICS::CalContect( MODELINFO& model, float timeStep )
{
	return true;
}

void DXM_PHYSICS::InitForce( XMFLOAT3& Force )
{
	Force.x = 0.0f;
	Force.y = 0.0f;
	Force.z = 0.0f;
}

void DXM_PHYSICS::SetGravityForce( XMFLOAT3& Force, float timeStep )
{
	Force.x += 0.0f;
	Force.y += m_GravityConstant;
	Force.z += 0.0f;
}

void DXM_PHYSICS::SetDragForce( XMFLOAT3& Force, XMFLOAT3 Velocity, float timeStep )
{
	Force.x -= Velocity.x * m_DragConstant;
	Force.y -= Velocity.y * m_DragConstant;
	Force.z -= Velocity.z * m_DragConstant;
}

void DXM_PHYSICS::SetFrictionForce( XMFLOAT3& Force, XMFLOAT3 Velocity, float timeStep )
{
	if ( m_Contect )
	{
		Force.x -= m_FrictionConstant * Velocity.x;
		Force.y -= m_FrictionConstant * Velocity.y;
		Force.z -= m_FrictionConstant * Velocity.z;
	}
}

void DXM_PHYSICS::SetCollisionForce( XMFLOAT3& Force, float timeStep )
{
	if ( m_Contect )
	{

	}
}

void DXM_PHYSICS::SetCollisionVelocity( XMFLOAT3& Velocity, float timeStep )
{

}