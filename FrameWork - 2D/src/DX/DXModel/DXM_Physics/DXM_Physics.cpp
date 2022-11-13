#include "DXM_Physics.h"

DXM_PHYSICS::DXM_PHYSICS() {}

DXM_PHYSICS::DXM_PHYSICS( const DXM_PHYSICS* Other )
{
	*this = *Other;
}

DXM_PHYSICS::~DXM_PHYSICS() {}

bool DXM_PHYSICS::Init( int Width, int Height, float gravity, float spring, float demper, float drag, float friction, float collision )
{
	m_GravityConstant = gravity;
	m_SpringConstant = spring;
	m_DemperConstant = demper;
	m_DragConstant = drag;
	m_FrictionConstant = friction;
	m_CollisionConstant = collision;

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
	m_Fence[2].OutNOR = DXMULTIPLY( m_Fence[1].InNOR, -1.0f * m_Fence[1].Outer );

	m_ERROR = 10.0f;
	return true;
}

bool DXM_PHYSICS::Frame( MODELINFO*& modelList, int numModel, double timeStep )
{
	InitForce( modelList, numModel );
	if ( !CalEulerMethod( modelList, numModel, timeStep ) ) { return false ; }

	return true;
}

void DXM_PHYSICS::Release()
{
	m_Fence = nullptr;
}


bool DXM_PHYSICS::CalEulerMethod( MODELINFO*& modelList, int numModel, double timeStep )
{
	if ( !CalAccelerate( modelList, numModel, timeStep ) ) { return false; }
	if ( !CalAngAccelerate( modelList, numModel, timeStep ) ) { return false; }

	if ( !CalVelocity( modelList, numModel, timeStep ) ) { return false; }
	if ( !CalAngVelocity( modelList, numModel, timeStep ) ) { return false; }

	for (int I = 0; I < numModel; I++ )
	{
		CalCollisionFence( modelList[ I ], timeStep );
		CalCollisionModel( modelList, modelList[ I ] , numModel, I, timeStep );
	}

	if ( !CalPosition( modelList, numModel, timeStep ) ) { return false; }
	if ( !CalAngle( modelList, numModel, timeStep ) ) { return false; }

	return true;
}


bool DXM_PHYSICS::CalAccelerate( MODELINFO*& modelList, int numModel, double timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		SetGravityForce( modelList[ I ].ACC, timeStep );
		SetDragForce( modelList[ I ].ACC, modelList[ I ].VEL, timeStep );
	}
	return true;
}

bool DXM_PHYSICS::CalVelocity( MODELINFO*& modelList, int numModel, double timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].VEL = DXADD( modelList[ I ].VEL, DXMULTIPLY( modelList[ I ].ACC, timeStep ) );
	}
	return true;
}

bool DXM_PHYSICS::CalPosition( MODELINFO*& modelList, int numModel, double timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].POS = DXADD( modelList[ I ].POS, DXMULTIPLY( modelList[ I ].VEL, timeStep ) );
	}
	return true;
}

bool DXM_PHYSICS::CalAngAccelerate( MODELINFO*& modelList, int numModel, double timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].ANGACC = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	}
	return true;
}

bool DXM_PHYSICS::CalAngVelocity( MODELINFO*& modelList, int numModel, double timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].ANGVEL = DXADD( modelList[ I ].ANGVEL, DXMULTIPLY( modelList[ I ].ANGACC, timeStep ) );
	}
	return true;
}

bool DXM_PHYSICS::CalAngle( MODELINFO*& modelList, int numModel, double timeStep )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].ANG = DXADD( modelList[ I ].ANG, DXMULTIPLY( modelList[ I ].ANGVEL, timeStep ) );
	}
	return true;
}

bool DXM_PHYSICS::CalCollisionFence( MODELINFO& model, double timeStep )
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
				SetCollisionForce( model, m_Fence[ I ].MODEL, timeStep );
			}
	}
	return true;
}


bool DXM_PHYSICS::CalCollisionModel( MODELINFO*& modelList, MODELINFO& model, int numModel, int index, double timeStep )
{
	for ( int I = index + 1; I < numModel; I++ )
	{
		XMFLOAT3 Position1 = DXSUBTRACT( modelList[ I ].POS, model.POS );
		XMFLOAT3 Position2 = DXMULTIPLY( Position1, -1.0f );
		if ( sqrt( DXDOT( Position1, Position1 ) ) <= 2 * 10.0f )
		{
			SetCollisionForce( model, modelList[ I ], timeStep );
		}
	}
	return true;
}


bool DXM_PHYSICS::CalFenceOut( MODELINFO model, MODELINFO Temp, double timeStep, float Bigger )
{
	/*
	Temp = model;
	CalEulerMethod( Temp, timeStep );
	if ( DXDOT( Temp.POS, Temp.POS ) > Bigger ) { return true; }
	else { return false; }
	*/
	return true;
}

void DXM_PHYSICS::InitForce( MODELINFO*& modelList, int numModel )
{
	for ( int I = 0; I < numModel; I++ )
	{
		modelList[ I ].ACC = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	}
}

void DXM_PHYSICS::SetGravityForce( XMFLOAT3& Force, double timeStep )
{
	XMFLOAT3 Gravity = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	Force = DXADD( Force, DXMULTIPLY( Gravity, m_GravityConstant ) );
}

void DXM_PHYSICS::SetDragForce( XMFLOAT3& Force, XMFLOAT3 Velocity, double timeStep )
{
	Force = DXSUBTRACT( Force, DXMULTIPLY( Velocity, m_DragConstant ) );
}

void DXM_PHYSICS::SetFrictionForce( XMFLOAT3& Force, XMFLOAT3 NorForce, double timeStep )
{
	Force = DXSUBTRACT( Force, DXMULTIPLY( NorForce, m_FrictionConstant ) );
}

void DXM_PHYSICS::SetCollisionForce( MODELINFO& model1, MODELINFO& model2, double timeStep )
{
	double Constant = model1.MASS * model2.MASS * ( m_CollisionConstant + 1 ) /  ( ( model1.MASS + model2.MASS ) * timeStep );

	XMFLOAT3 unitForce1 = DXUNIT( DXSUBTRACT( model1.POS, model2.POS ) );
	XMFLOAT3 unitForce2 = DXUNIT( DXSUBTRACT( model2.POS, model1.POS ) );

	XMFLOAT3 norForce1 = DXMULTIPLY( unitForce1, Constant * DXDOT( DXSUBTRACT( model2.VEL, model1.VEL ), unitForce1 ) );
	XMFLOAT3 norForce2 = DXMULTIPLY( unitForce2, Constant * DXDOT( DXSUBTRACT( model1.VEL, model2.VEL ), unitForce2 ) );

	if ( DXDOT( unitForce1, model1.VEL ) < 0 || DXDOT( unitForce2, model2.VEL ) < 0 )
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

/*

bool CalFenceSafety()
{
	for (int I = 0; I < m_NumFence; I++ )
	{
		if ( DXDOT( model.POS, DXUNIT( m_Fence[ I ].OutNOR ) ) > 0.0f )
		{
			float Bigger = m_Fence[ I ].Outer * m_Fence[ I ].Outer;
			float Smaller = ( m_Fence[ I ].Outer - m_Fence[ I ].Range ) * ( m_Fence[ I ].Outer - m_Fence[ I ].Range );
			float Value = (float)pow( DXDOT( model.POS, DXUNIT( m_Fence[ I ].OutNOR ) ), 2);
			if ( Value <= Bigger && Value >= Smaller && DXDOT( model.VEL, m_Fence[ I ].InNOR ) < 0 )
			{
				m_Fence[ I ].MODEL.MASS = 10000.0f * model.MASS;
				XMFLOAT3 Unit = DXUNIT( m_Fence[ I ].OutNOR );
				XMFLOAT3 Position = DXMULTIPLY( Unit, DXDOT( Unit, m_Fence[ I ].OutNOR ) - DXDOT( Unit, model.POS ) );
				m_Fence[ I ].MODEL.POS = DXADD( model.POS, Position );
				SetCollisionForce( model, m_Fence[ I ].MODEL, timeStep );
			}
			else if ( DXDOT( model.POS, model.POS ) < Smaller )
			{
				MODELINFO* Temp = new MODELINFO;
				if ( CalFenceOut( model, *Temp, timeStep, Bigger ) )
				{
					while( true )
					{
						if ( CalFenceOut( model, *Temp, timeStep, Bigger ) )
						{
							timeStep *= 2;
						}
						else
						{
							CalEulerMethod( model, timeStep );
							break;
						}
					}
				}
				delete Temp;
			}
		}
	}
	return true;
}
*/