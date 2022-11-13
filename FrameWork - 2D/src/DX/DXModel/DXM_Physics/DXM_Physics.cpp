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
	m_Fence[0].OutNOR.x = - m_Fence[0].InNOR.x * m_Fence[0].Outer;
	m_Fence[0].OutNOR.y = - m_Fence[0].InNOR.y * m_Fence[0].Outer;
	m_Fence[0].OutNOR.z = - m_Fence[0].InNOR.z * m_Fence[0].Outer;
	// Left
	m_Fence[1].Range = 10.0f;
	m_Fence[1].Outer = (float)Width/2;
	m_Fence[1].InNOR = XMFLOAT3( 1.0f, 0.0f, 0.0f );
	m_Fence[1].OutNOR.x = - m_Fence[1].InNOR.x * m_Fence[1].Outer;
	m_Fence[1].OutNOR.y = - m_Fence[1].InNOR.y * m_Fence[1].Outer;
	m_Fence[1].OutNOR.z = - m_Fence[1].InNOR.z * m_Fence[1].Outer;
	// Right
	m_Fence[2].Range = 10.0f;
	m_Fence[2].Outer = (float)Width/2;
	m_Fence[2].InNOR = XMFLOAT3( -1.0f, 0.0f, 0.0f );
	m_Fence[2].OutNOR.x = - m_Fence[2].InNOR.x * m_Fence[2].Outer;
	m_Fence[2].OutNOR.y = - m_Fence[2].InNOR.y * m_Fence[2].Outer;
	m_Fence[2].OutNOR.z = - m_Fence[2].InNOR.z * m_Fence[2].Outer;

	m_ERROR = 0.01f;

	return true;
}

bool DXM_PHYSICS::Frame( MODELINFO*& modelList, int numModel, double timeStep )
{
	InitForce( modelList, numModel );
	for ( int I = 0; I < numModel; I++ )
	{
		m_Collision = false;
		m_Contact = false;
		if ( !CalEulerMethod( modelList, modelList[ I ], numModel, I, timeStep ) ) { return false ; }
	}

	return true;
}

void DXM_PHYSICS::Release()
{
	m_Fence = nullptr;
}


bool DXM_PHYSICS::CalEulerMethod( MODELINFO*& modelList, MODELINFO& model, int numModel, int index, double timeStep )
{
	if ( !CalAccelerate( modelList, model, numModel, index, timeStep ) ) { return false; }
	if ( !CalVelocity( model, timeStep ) ) { return false; }

	if ( !CalAngAccelerate( model, timeStep ) ) { return false; }
	if ( !CalAngVelocity( model, timeStep ) ) { return false; }

	if ( !CalPosition( model, timeStep ) ) { return false; }
	if ( !CalAngle( model, timeStep ) ) { return false; }

	return true;
}


bool DXM_PHYSICS::CalAccelerate( MODELINFO*& modelList, MODELINFO& model, int numModel, int index, double timeStep )
{
	SetGravityForce( model.ACC, timeStep );
	SetDragForce( model.ACC, model.VEL, timeStep );
	CalCollisionFence( model, timeStep );
	CalCollisionModel( modelList, model, numModel, index, timeStep );
	return true;
}

bool DXM_PHYSICS::CalVelocity( MODELINFO& model, double timeStep )
{
	model.VEL = XMFLOAT3( model.VEL.x + model.ACC.x * timeStep,
		model.VEL.y + model.ACC.y * timeStep,
		model.VEL.z + model.ACC.z * timeStep );
	return true;
}

bool DXM_PHYSICS::CalPosition( MODELINFO& model, double timeStep )
{
	model.POS = XMFLOAT3( model.POS.x + model.VEL.x * timeStep,
		model.POS.y + model.VEL.y * timeStep,
		model.POS.z + model.VEL.z * timeStep );
	return true;
}

bool DXM_PHYSICS::CalAngAccelerate( MODELINFO& model, double timeStep )
{
	model.ANGACC = XMFLOAT3( model.ANGACC.x, model.ANGACC.y, model.ACC.z );
	return true;
}

bool DXM_PHYSICS::CalAngVelocity( MODELINFO& model, double timeStep )
{
	model.ANGVEL = XMFLOAT3( model.ANGVEL.x + model.ANGACC.x * timeStep,
		model.ANGVEL.y + model.ANGACC.y * timeStep,
		model.ANGVEL.z + model.ANGACC.z * timeStep );
	return true;
}

bool DXM_PHYSICS::CalAngle( MODELINFO& model, double timeStep )
{
	model.ANG = XMFLOAT3( model.ANG.x + model.ANGVEL.x * timeStep,
		model.ANG.y + model.ANGVEL.y * timeStep,
		model.ANG.z + model.ANGVEL.z * timeStep );
	return true;
}

bool DXM_PHYSICS::CalCollisionFence( MODELINFO& model, double timeStep )
{
	for (int I = 0; I < m_NumFence; I++ )
	{
		if ( DXDOT( model.POS, DXUNIT( m_Fence[ I ].OutNOR ) ) > 0.0f )
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
	XMFLOAT3 Gravity = XMFLOAT3( 0.0f, 1.0f, 0.0f );
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
	XMFLOAT3 unitForce1 = DXUNIT( DXSUBTRACT( model1.POS, model2.POS ) );
	XMFLOAT3 unitForce2 = DXUNIT( DXSUBTRACT( model2.POS, model1.POS ) );
	XMFLOAT3 unitNor1 = XMFLOAT3( 0.0f, 0.0f, -1.0f );
	XMFLOAT3 unitNor2 = DXMULTIPLY( unitNor1, -1.0f );
	XMFLOAT3 unitLin1 = DXUNIT( DXCROSS( unitNor1, unitForce1 ) );
	XMFLOAT3 unitLin2 = DXUNIT( DXCROSS( unitNor2, unitForce2 ) );

	double Constant = model1.MASS * model2.MASS * ( m_CollisionConstant + 1 ) /  ( ( model1.MASS + model2.MASS ) * timeStep );
	XMFLOAT3 norForce1 = DXMULTIPLY( unitForce1, Constant * DXDOT( DXSUBTRACT( model2.VEL, model1.VEL ), unitForce1 ) );
	XMFLOAT3 norForce2 = DXMULTIPLY( norForce1, -1.0f );

	XMFLOAT3 fricForce1 = DXMULTIPLY( unitLin1, -1.0f * m_FrictionConstant * sqrt( DXDOT( norForce1, norForce1 ) ) );
	XMFLOAT3 fricForce2 = DXMULTIPLY( unitLin2, -1.0f * m_FrictionConstant * sqrt( DXDOT( norForce2, norForce2 ) ) );

	if ( DXDOT( unitForce1, norForce1 ) > 0  && DXDOT( unitForce2, norForce2 ) > 0 )
	{
		if ( abs( DXDOT( unitForce1, model1.VEL ) ) <= m_ERROR && abs( DXDOT( unitForce2, model2.VEL ) ) <= m_ERROR )
		{
			model1.VEL = DXADD( model1.VEL, DXMULTIPLY( unitForce1, -1.0f * DXDOT( model1.VEL, unitForce1) ) );
			model2.VEL = DXADD( model2.VEL, DXMULTIPLY( unitForce2, -1.0f * DXDOT( model2.VEL, unitForce2) ) );
		}
		XMFLOAT3 norForce1 = DXMULTIPLY( unitForce1, Constant * DXDOT( DXSUBTRACT( model2.VEL, model1.VEL ), unitForce1 ) );
		XMFLOAT3 norForce2 = DXMULTIPLY( norForce1, -1.0f );

		model1.ACC = DXADD( DXMULTIPLY( model1.ACC, -1.0f ), DXDIVIDE( norForce1, model1.MASS ) );
		model2.ACC = DXADD( DXMULTIPLY( model2.ACC, -1.0f ), DXDIVIDE( norForce2, model2.MASS ) );
		/*
		model1.ACC= DXADD( model1.ACC, DXDIVIDE( fricForce1, model1.MASS ) );
		model2.ACC= DXADD( model2.ACC, DXDIVIDE( fricForce2, model2.MASS ) );
		*/
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