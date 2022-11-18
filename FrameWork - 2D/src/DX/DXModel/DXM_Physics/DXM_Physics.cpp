#include "DXM_Physics.h"

DXM_PHYSICS::DXM_PHYSICS() {}

DXM_PHYSICS::DXM_PHYSICS( const DXM_PHYSICS* Other )
{
	*this = *Other;
}

DXM_PHYSICS::~DXM_PHYSICS() {}



bool DXM_PHYSICS::Init( int Width, int Height )
{
	// Set Constant
	m_GravityConstant = 100.0f;
	m_SpringConstant = 1000.0f;
	m_DemperConstant = 5.0f;
	m_DragConstant = 0.5f;
	m_CollisionConstant = 0.2f;

	m_ERROR = 0.01f;

	m_NumFence = 3;
	m_Fence = new FENCE[ m_NumFence ];
	// Bottom
	m_Fence[0].Range = 10.0f;
	m_Fence[0].Outer = (float)Height/2;
	m_Fence[0].InNOR = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	m_Fence[0].OutNOR = XMFLOAT3( 0.0f, -1.0f, 0.0f );
	m_Fence[0].MODEL.POS = XMFLOAT3( 0.0f, -1.0f * m_Fence[0].Outer, 0.0f );
	// Left
	m_Fence[1].Range = 10.0f;
	m_Fence[1].Outer = (float)Width/2;
	m_Fence[1].InNOR = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	m_Fence[1].OutNOR = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	m_Fence[1].MODEL.POS = XMFLOAT3( -1.0f * m_Fence[1].Outer, 0.0f, 0.0f );
	// Right
	m_Fence[2].Range = 10.0f;
	m_Fence[2].Outer = (float)Width/2;
	m_Fence[2].InNOR = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	m_Fence[2].OutNOR = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	m_Fence[2].MODEL.POS = XMFLOAT3( 1.0f * m_Fence[2].Outer, 0.0f, 0.0f );

	return true;
}

bool DXM_PHYSICS::Frame( MODELINFO*& modelList, int numModel, float timeStep )
{
	CalEulerMethod( modelList, numModel, timeStep, true );

	return true;
}

void DXM_PHYSICS::Release()
{
	m_Fence = nullptr;
}


void DXM_PHYSICS::CalEulerMethod( MODELINFO*& modelList, int numModel, float timeStep, bool collisionState )
{
	for ( int I = 0; I < numModel; I++ )
	{
		CalAccelerate( modelList[ I ], timeStep );
		CalAngAccelerate( modelList[ I ], timeStep );
	}

	for (int I = 0; I < numModel; I++ )
	{
		CalCollisionModel( modelList, modelList[ I ] , numModel, I, timeStep );
		CalCollisionFence( modelList[ I ], timeStep );
	}

	for ( int I = 0; I < numModel; I++ )
	{
		CalVelocity( modelList[ I ], timeStep );
		CalAngVelocity( modelList[ I ], timeStep );
	}

	for ( int I = 0; I < numModel; I++ )
	{
		CalPosition( modelList[ I ], timeStep );
		CalAngle( modelList[ I ], timeStep );
	}

}



void DXM_PHYSICS::CalCollision( MODELINFO*& modelList, int numModel, float timeStep, bool collisionState )
{
}

void DXM_PHYSICS::CalCollisionFence( MODELINFO& model, float timeStep )
{
	for (int I = 0; I < m_NumFence; I++ )
	{
		float Time = timeStep;
		CalLength( m_Fence[ I ], model );
		m_Fence[ I ].MODEL.MASS = 10000.0f * model.MASS;
		XMFLOAT3 Length = DXSUBTRACT( m_Fence[ I ].MODEL.POS, model.POS );

		if ( DXDOT( Length, Length ) <= 4 * 100.0f )
		{
			SetCollisionVelocity( model, m_Fence[ I ].MODEL, Time );
			SetSpringForce( model, m_Fence[ I ].MODEL );
		}
	}
}

void DXM_PHYSICS::CalLength( FENCE& fence, MODELINFO& model )
{
	XMFLOAT3 temp = fence.OutNOR;
	temp =  DXMULTIPLY( temp, DXDOT( temp, DXSUBTRACT( fence.MODEL.POS, model.POS ) ) );
	fence.MODEL.POS = DXADD( temp, model.POS );
}

void DXM_PHYSICS::NextFrame( MODELINFO& model, float timeStep )
{
	CalAccelerate( model, timeStep );
	CalAngAccelerate( model, timeStep );
	CalVelocity( model, timeStep );
	CalAngVelocity( model, timeStep );
	CalPosition( model, timeStep );
	CalAngle( model, timeStep );
}


void DXM_PHYSICS::CalCollisionModel( MODELINFO*& modelList, MODELINFO& model, int numModel, int index, float timeStep )
{
	for ( int I = index + 1; I < numModel; I++ )
	{
		float Time = timeStep;
		XMFLOAT3 Length = DXSUBTRACT( modelList[ I ].POS, model.POS );
		if ( DXDOT( Length, Length ) <= 4 * 100.0f )
		{
			SetCollisionVelocity( model, modelList[ I ], Time );
			SetSpringForce( model, modelList[ I ] );
		}
	}
}



void DXM_PHYSICS::CalAccelerate( MODELINFO& model, float timeStep )
{
	model.ACC = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	SetGravityForce( model.ACC, timeStep );
	SetDragForce( model.ACC, model.VEL, timeStep );

}

void DXM_PHYSICS::CalAngAccelerate( MODELINFO& model, float timeStep )
{
	model.ANGACC = XMFLOAT3( 0.0f, 0.0f, 0.0f );
}


void DXM_PHYSICS::CalVelocity( MODELINFO& model, float timeStep ) { model.VEL = DXADD( model.VEL, DXMULTIPLY( model.ACC, timeStep ) ); }
void DXM_PHYSICS::CalAngVelocity( MODELINFO& model, float timeStep ) { model.ANGVEL = DXADD( model.ANGVEL, DXMULTIPLY( model.ANGACC, timeStep ) ); }
void DXM_PHYSICS::CalPosition( MODELINFO& model, float timeStep ) { model.POS = DXADD( model.POS, DXMULTIPLY( model.VEL, timeStep ) ); }
void DXM_PHYSICS::CalAngle( MODELINFO& model, float timeStep ) { model.ANG = DXADD( model.ANG, DXMULTIPLY( model.ANGVEL, timeStep ) ); }

void DXM_PHYSICS::SetGravityForce( XMFLOAT3& Force, float timeStep ) { Force = DXADD( Force, DXMULTIPLY( XMFLOAT3( 0.0f, -1.0f, 0.0f ), m_GravityConstant ) ); }

void DXM_PHYSICS::SetDragForce( XMFLOAT3& Force, XMFLOAT3 Velocity, float timeStep ) { Force = DXSUBTRACT( Force, DXMULTIPLY( Velocity, m_DragConstant ) ); }

void DXM_PHYSICS::SetSpringForce( MODELINFO& model1, MODELINFO& model2 )
{
	XMFLOAT3 Temp = DXSUBTRACT( model1.POS, model2.POS );

	XMFLOAT3 unitForce1 = DXUNIT( DXSUBTRACT( model1.POS, model2.POS ) );
	XMFLOAT3 unitForce2 = DXUNIT( DXSUBTRACT( model2.POS, model1.POS ) );

	float Constant = 2.0f * 10.0f - sqrt( DXDOT( Temp, Temp ) );
	if ( Constant <= 0.0f )
	{
		Constant = 0.0f;
	}
	XMFLOAT3 springForce1 = DXMULTIPLY( unitForce1, m_SpringConstant * Constant );
	XMFLOAT3 springForce2 = DXMULTIPLY( unitForce2, m_SpringConstant * Constant );

	springForce1 = DXADD( springForce1, DXMULTIPLY( unitForce1, -1.0f * m_DemperConstant * DXDOT( DXSUBTRACT( model2.VEL, model1.VEL ), unitForce1 ) ) );
	springForce2 = DXADD( springForce2, DXMULTIPLY( unitForce2, -1.0f * m_DemperConstant * DXDOT( DXSUBTRACT( model1.VEL, model2.VEL ), unitForce2 ) ) );

	model1.ACC = DXADD( model1.ACC, springForce1 );
	model2.ACC = DXADD( model2.ACC, springForce2 );
}

void DXM_PHYSICS::SetCollisionVelocity( MODELINFO& model1, MODELINFO& model2, float timeStep )
{

	XMFLOAT3 unitForce1 = DXUNIT( DXSUBTRACT( model1.POS, model2.POS ) );
	XMFLOAT3 unitForce2 = DXUNIT( DXSUBTRACT( model2.POS, model1.POS ) );

	XMFLOAT3 Temp = DXSUBTRACT( model2.VEL, model1.VEL );
	XMFLOAT3 norForce1 = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 norForce2 = XMFLOAT3( 0.0f, 0.0f, 0.0f );

	float Constant;

	if( DXDOT( unitForce1, Temp ) >= 0.0f )
	{
		Constant = model1.MASS * model2.MASS * ( m_CollisionConstant + 1 ) /  ( ( model1.MASS + model2.MASS ) * timeStep );
		norForce1 = DXMULTIPLY( unitForce1, Constant * DXDOT( DXSUBTRACT( model2.VEL, model1.VEL ), unitForce1 ) );
		norForce2 = DXMULTIPLY( unitForce2, Constant * DXDOT( DXSUBTRACT( model1.VEL, model2.VEL ), unitForce2 ) );
	}

	model1.ACC = DXADD( model1.ACC, DXDIVIDE( norForce1, model1.MASS ) );
	model2.ACC = DXADD( model2.ACC, DXDIVIDE( norForce2, model2.MASS ) );
}