#ifndef __DXM_PHYSICS_H__
#define __DXM_PHYSICS_H__

#include "DXM_Common.h"
#include <cmath>

struct FENCE
{
	float Range;
	float Outer;
	XMFLOAT3 NOR;
	XMFLOAT3 DOT;
};

class DXM_PHYSICS
{
	public :

		DXM_PHYSICS();
		DXM_PHYSICS( const DXM_PHYSICS* );
		~DXM_PHYSICS();

	public :

		bool Init( int, int, float, float, float, float, float );
		bool Frame( MODELINFO*&, int, float );
		void Release();

	private :

		bool CalEulerMethod( MODELINFO&, float );

		bool CalAccelerate( MODELINFO&, float );
		bool CalVelocity( MODELINFO&, float );
		bool CalPosition( MODELINFO&, float );
		bool CalAngAccelerate( MODELINFO&, float );
		bool CalAngVelocity( MODELINFO&, float );
		bool CalAngle( MODELINFO&, float );

		bool CalCollision( MODELINFO&, float );
		bool CalContect( MODELINFO&, float );

		void InitForce( XMFLOAT3& Force );
		void SetGravityForce( XMFLOAT3&, float );
		void SetDragForce( XMFLOAT3&, XMFLOAT3, float );
		void SetFrictionForce( XMFLOAT3&, XMFLOAT3, float );
		void SetCollisionForce( XMFLOAT3&, float );
		void SetCollisionVelocity( XMFLOAT3&, float );

		float m_GravityConstant;
		float m_SpringConstant;
		float m_DemperConstant;
		float m_DragConstant;
		float m_FrictionConstant;
		float m_ERROR;

		bool m_Collision;
		bool m_Contect;

		int m_NumFence;
		FENCE* m_Fence;
};

#endif