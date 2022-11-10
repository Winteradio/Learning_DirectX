#include "DXM_Physics.h"

DXM_PHYSICS::DXM_PHYSICS() {}

DXM_PHYSICS::DXM_PHYSICS( const DXM_PHYSICS* Other )
{
	*this = *Other;
}

DXM_PHYSICS::~DXM_PHYSICS() {}

bool DXM_PHYSICS::Init( float gravity, float spring, float demper, float drag )
{
	m_GravityConstant = gravity;
	m_SpringConstant = spring;
	m_DemperConstant = demper;
	m_DragConstant = drag;

	return true;
}

bool DXM_PHYSICS::Frame( MODELINFO*& modelList, int numModel, float timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		LOG_INFO(" %d \n ", I );
		if ( !CalAccelerate( modelList[ I ], timeStep ) ) { return false; }
		if ( !CalVelocity( modelList[ I ], timeStep ) ) { return false; }

		if ( !CalAngAccelerate( modelList[ I ], timeStep ) ) { return false; }
		if ( !CalAngVelocity( modelList[ I ], timeStep ) ) { return false; }

		if ( !CalPosition( modelList[ I ], timeStep ) ) { return false; }
		if ( !CalAngle( modelList[ I ], timeStep ) ) { return false; }
	}

	return true;
}

bool DXM_PHYSICS::CalAccelerate( MODELINFO& model, float timeStep )
{
	XMFLOAT3 gravity = XMFLOAT3( 0.0f, m_GravityConstant, 0.0f );
	model.ACC = XMFLOAT3( model.ACC.x + gravity.x, model.ACC.y + gravity.y, model.ACC.z + gravity.z );
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
