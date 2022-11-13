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
	m_SpringConstant = 0.0f;
	m_DemperConstant = 0.0f;
	m_DragConstant = 0.1f;
	m_CollisionConstant = 0.7f;

	m_ERROR = 10.0f;

	m_NumFence = 3;
	m_Fence = new FENCE[ m_NumFence ];
	// Bottom
	m_Fence[0].Range = 10.0f;
	m_Fence[0].Outer = (float)Height/2;
	m_Fence[0].InNOR = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	m_Fence[0].OutNOR = DXMULTIPLY( m_Fence[0].InNOR, -1.0f * m_Fence[0].Outer );
	// Left
	m_Fence[1].Range = 10.0f;
	m_Fence[1].Outer = (float)Width/2;
	m_Fence[1].InNOR = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	m_Fence[1].OutNOR = DXMULTIPLY( m_Fence[1].InNOR, -1.0f * m_Fence[1].Outer );
	// Right
	m_Fence[2].Range = 10.0f;
	m_Fence[2].Outer = (float)Width/2;
	m_Fence[2].InNOR = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	m_Fence[2].OutNOR = DXMULTIPLY( m_Fence[2].InNOR, -1.0f * m_Fence[2].Outer );

	return true;
}

bool DXM_PHYSICS::Frame( MODELINFO*& modelList, int numModel, float timeStep )
{
	InitForce( modelList, numModel );
	CalEulerMethod( modelList, numModel, timeStep, true );

	return true;
}

void DXM_PHYSICS::Release()
{
	m_Fence = nullptr;
}



void DXM_PHYSICS::CalEulerMethod( MODELINFO*& modelList, int numModel, float timeStep, bool collisionState )
{
	CalAccelerate( modelList, numModel, timeStep );
	CalAngAccelerate( modelList, numModel, timeStep );

	CalVelocity( modelList, numModel, timeStep );
	CalAngVelocity( modelList, numModel, timeStep );

	CalCollision( modelList, numModel, timeStep, collisionState );

	CalPosition( modelList, numModel, timeStep );
	CalAngle( modelList, numModel, timeStep );
}



void DXM_PHYSICS::CalCollision( MODELINFO*& modelList, int numModel, float timeStep, bool collisionState )
{
	if ( collisionState )
	{
		for (int I = 0; I < numModel; I++ )
		{
			CalCollisionFence( modelList[ I ], timeStep );
			CalCollisionModel( modelList, modelList[ I ] , numModel, I, timeStep );
		}
	}
}

void DXM_PHYSICS::CalCollisionFence( MODELINFO& model, float timeStep )
{
	for (int I = 0; I < m_NumFence; I++ )
	{
			float Bigger = m_Fence[ I ].Outer * m_Fence[ I ].Outer;
			float Smaller = ( m_Fence[ I ].Outer - m_Fence[ I ].Range ) * ( m_Fence[ I ].Outer - m_Fence[ I ].Range );
			float Value = (float)pow( DXDOT( model.POS, DXUNIT( m_Fence[ I ].OutNOR ) ), 2);
			if ( Value <= Bigger && Value >= Smaller )
			{
				m_Fence[ I ].MODEL.MASS = 10000.0f * model.MASS;
				XMFLOAT3 Unit = DXUNIT( m_Fence[ I ].OutNOR );
				XMFLOAT3 Position = DXMULTIPLY( Unit, DXDOT( Unit, m_Fence[ I ].OutNOR ) - DXDOT( Unit, model.POS ) );
				m_Fence[ I ].MODEL.POS = DXADD( model.POS, Position );
				SetCollisionVelocity( model, m_Fence[ I ].MODEL, timeStep );
			}
	}
}

void DXM_PHYSICS::CalCollisionModel( MODELINFO*& modelList, MODELINFO& model, int numModel, int index, float timeStep )
{
	for ( int I = index + 1; I < numModel; I++ )
	{
		XMFLOAT3 Length = DXSUBTRACT( modelList[ I ].POS, model.POS );
		if ( DXDOT( Length, Length ) <= 4 * 100.0f )
		{
			SetCollisionVelocity( model, modelList[ I ], timeStep );
		}
		/*
		else
		{
			MODELINFO* TempList = new MODELINFO[ numModel ];
			TempList = modelList;
			if ( )
			{
				CalEulerMethod( modelList, numModel, timeStep / 2, false );
			}
		}
		*/
	}
}



void DXM_PHYSICS::CalAccelerate( MODELINFO*& modelList, int numModel, float timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		SetGravityForce( modelList[ I ].ACC, timeStep );
		SetDragForce( modelList[ I ].ACC, modelList[ I ].VEL, timeStep );
	}
}

void DXM_PHYSICS::CalAngAccelerate( MODELINFO*& modelList, int numModel, float timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].ANGACC = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	}
}



void DXM_PHYSICS::CalVelocity( MODELINFO*& modelList, int numModel, float timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].VEL = DXADD( modelList[ I ].VEL, DXMULTIPLY( modelList[ I ].ACC, timeStep ) );
	}
}

void DXM_PHYSICS::CalAngVelocity( MODELINFO*& modelList, int numModel, float timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].ANGVEL = DXADD( modelList[ I ].ANGVEL, DXMULTIPLY( modelList[ I ].ANGACC, timeStep ) );
	}
}



void DXM_PHYSICS::CalPosition( MODELINFO*& modelList, int numModel, float timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].POS = DXADD( modelList[ I ].POS, DXMULTIPLY( modelList[ I ].VEL, timeStep ) );
	}
}

void DXM_PHYSICS::CalAngle( MODELINFO*& modelList, int numModel, float timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].ANG = DXADD( modelList[ I ].ANG, DXMULTIPLY( modelList[ I ].ANGVEL, timeStep ) );
	}
}



void DXM_PHYSICS::InitForce( MODELINFO*& modelList, int numModel )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].ACC = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	}
}

void DXM_PHYSICS::SetGravityForce( XMFLOAT3& Force, float timeStep )
{
	XMFLOAT3 Gravity = XMFLOAT3( 0.0f, -1.0f, 0.0f );
	Force = DXADD( Force, DXMULTIPLY( Gravity, m_GravityConstant ) );
}

void DXM_PHYSICS::SetDragForce( XMFLOAT3& Force, XMFLOAT3 Velocity, float timeStep )
{
	Force = DXSUBTRACT( Force, DXMULTIPLY( Velocity, m_DragConstant ) );
}

void DXM_PHYSICS::SetCollisionVelocity( MODELINFO& model1, MODELINFO& model2, float timeStep )
{
	float Constant = model1.MASS * model2.MASS * ( m_CollisionConstant + 1 ) /  ( ( model1.MASS + model2.MASS ) * timeStep );

	XMFLOAT3 unitForce1 = DXUNIT( DXSUBTRACT( model1.POS, model2.POS ) );
	XMFLOAT3 unitForce2 = DXUNIT( DXSUBTRACT( model2.POS, model1.POS ) );

	XMFLOAT3 norForce1 = DXMULTIPLY( unitForce1, Constant * DXDOT( DXSUBTRACT( model2.VEL, model1.VEL ), unitForce1 ) );
	XMFLOAT3 norForce2 = DXMULTIPLY( unitForce2, Constant * DXDOT( DXSUBTRACT( model1.VEL, model2.VEL ), unitForce2 ) );

	if ( DXDOT( unitForce1, norForce1 ) > 0.0f && DXDOT( unitForce2, norForce2 ) > 0.0f )
	{
		XMFLOAT3 linVEL1 = DXADD( model1.VEL, DXMULTIPLY( unitForce1, -1.0f * DXDOT( model1.VEL, unitForce1) ) );
		XMFLOAT3 linVEL2 = DXADD( model2.VEL, DXMULTIPLY( unitForce2, -1.0f * DXDOT( model2.VEL, unitForce2) ) );

		XMFLOAT3 norVEL1 = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		XMFLOAT3 norVEL2 = XMFLOAT3( 0.0f, 0.0f, 0.0f );

		if ( abs( DXDOT( unitForce1, model1.VEL ) ) >= m_ERROR || abs( DXDOT( unitForce2, model2.VEL ) ) >= m_ERROR  )
		{
			XMFLOAT3 tempVEL1 = DXMULTIPLY( unitForce1, DXDOT( unitForce1, model1.VEL ) );
			XMFLOAT3 tempVEL2 = DXMULTIPLY( unitForce2, DXDOT( unitForce2, model2.VEL ) );
			norVEL1 = DXADD( DXMULTIPLY( tempVEL1, ( model1.MASS - m_CollisionConstant * model2.MASS ) / ( model1.MASS + model2.MASS ) ), DXMULTIPLY( tempVEL2, ( 1 + m_CollisionConstant ) * model2.MASS / ( model1.MASS + model2.MASS ) ) );
			norVEL2 = DXADD( DXMULTIPLY( tempVEL1, ( 1 + m_CollisionConstant ) * model1.MASS / ( model1.MASS + model2.MASS ) ), DXMULTIPLY( tempVEL2, ( model2.MASS - m_CollisionConstant * model1.MASS ) / ( model1.MASS + model2.MASS ) ) );
		}

		model1.VEL = DXADD( linVEL1, norVEL1 );
		model2.VEL = DXADD( linVEL2, norVEL2 );
	}
}