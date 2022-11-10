#ifndef __DXM_PHYSICS_H__
#define __DXM_PHYSICS_H__

#include "DXM_Common.h"
#include <cmath>

struct PHYDATA
{
	XMFLOAT3 FOR;
	XMFLOAT3 VEL;
	XMFLOAT3 POS;
	float MASS;
};

class DXM_PHYSICS
{
	public :

		DXM_PHYSICS();
		DXM_PHYSICS( const DXM_PHYSICS* );
		~DXM_PHYSICS();

	public :

		bool Init( float, float, float, float );
		bool Frame( MODELINFO*&, int, float );

	private :

		bool CalAccelerate( MODELINFO&, float );
		bool CalVelocity( MODELINFO&, float );
		bool CalPosition( MODELINFO&, float );
		bool CalAngAccelerate( MODELINFO&, float );
		bool CalAngVelocity( MODELINFO&, float );
		bool CalAngle( MODELINFO&, float );

		bool CalCollision( MODELINFO&, float );
		bool CalContect( MODELINFO&, float );

		float m_GravityConstant;
		float m_SpringConstant;
		float m_DemperConstant;
		float m_DragConstant;
};

#endif