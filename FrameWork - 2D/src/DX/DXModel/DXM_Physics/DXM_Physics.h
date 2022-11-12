#ifndef __DXM_PHYSICS_H__
#define __DXM_PHYSICS_H__

#include "DXM_Common.h"
#include <cmath>

struct FENCE
{
	float Range;
	float Outer;
	XMFLOAT3 InNOR;
	XMFLOAT3 OutNOR;

	MODELINFO MODEL;
};

class DXM_PHYSICS
{
	public :

		DXM_PHYSICS();
		DXM_PHYSICS( const DXM_PHYSICS* );
		~DXM_PHYSICS();

	public :

		bool Init( int, int, float, float, float, float, float, float );
		bool Frame( MODELINFO*&, int, double );
		void Release();

	private :

		bool CalEulerMethod( MODELINFO&, double );

		bool CalAccelerate( MODELINFO&, double );
		bool CalVelocity( MODELINFO&, double );
		bool CalPosition( MODELINFO&, double );
		bool CalAngAccelerate( MODELINFO&, double );
		bool CalAngVelocity( MODELINFO&, double );
		bool CalAngle( MODELINFO&, double );

		bool CalCollision( MODELINFO&, double );
		bool CalContact( MODELINFO&, double );
		bool CalFenceOut( MODELINFO, MODELINFO, double, float );

		void InitForce( XMFLOAT3& Force );
		void SetGravityForce( XMFLOAT3&, double );
		void SetDragForce( XMFLOAT3&, XMFLOAT3, double );
		void SetFrictionForce( XMFLOAT3&, XMFLOAT3, double );
		void SetCollisionForce( MODELINFO&, MODELINFO&, double );
		void SetCollisionVelocity( XMFLOAT3&, double );

		float m_GravityConstant;
		float m_SpringConstant;
		float m_DemperConstant;
		float m_DragConstant;
		float m_FrictionConstant;
		float m_CollisionConstant;
		float m_ERROR;

		bool m_Collision;
		bool m_Contact;

		int m_NumFence;
		FENCE* m_Fence;
};

#endif