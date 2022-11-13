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

		bool CalEulerMethod( MODELINFO*&, int, double );

		bool CalAccelerate( MODELINFO*&, int, double );
		bool CalVelocity( MODELINFO*&, int, double );
		bool CalPosition( MODELINFO*&, int, double );
		bool CalAngAccelerate( MODELINFO*&, int, double );
		bool CalAngVelocity( MODELINFO*&, int, double );
		bool CalAngle( MODELINFO*&, int, double );

		bool CalCollisionFence( MODELINFO&, double );
		bool CalCollisionModel( MODELINFO*&, MODELINFO&, int, int, double );
		bool CalFenceOut( MODELINFO, MODELINFO, double, float );

		void InitForce( MODELINFO*&, int );
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