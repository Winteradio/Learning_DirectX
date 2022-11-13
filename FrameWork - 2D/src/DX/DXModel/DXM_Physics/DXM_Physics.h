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
		bool Frame( MODELINFO*&, int, float );
		void Release();

	private :

		bool CalEulerMethod( MODELINFO*&, int, float );

		bool CalAccelerate( MODELINFO*&, int, float );
		bool CalVelocity( MODELINFO*&, int, float );
		bool CalPosition( MODELINFO*&, int, float );
		bool CalAngAccelerate( MODELINFO*&, int, float );
		bool CalAngVelocity( MODELINFO*&, int, float );
		bool CalAngle( MODELINFO*&, int, float );

		bool CalCollisionFence( MODELINFO&, float );
		bool CalCollisionModel( MODELINFO*&, MODELINFO&, int, int, float );
		bool CalFenceOut( MODELINFO, MODELINFO, float, float );

		void InitForce( MODELINFO*&, int );
		void SetGravityForce( XMFLOAT3&, float );
		void SetDragForce( XMFLOAT3&, XMFLOAT3, float );
		void SetCollisionVelocity( MODELINFO&, MODELINFO&, float );

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