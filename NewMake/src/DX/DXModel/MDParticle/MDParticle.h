#ifndef __MDPARTICLE_H__
#define __MDPARTICLE_H__

#include "DXCommon.h"

struct MDPARTICLE
{
	public:
		MDPARTICLE();
		MDPARTICLE( const MDPARTICLE* );
		MDPARTICLE( XMFLOAT3, XMFLOAT4 );
		MDPARTICLE( float, XMFLOAT3, XMFLOAT3, XMFLOAT3, XMFLOAT4 );
		~MDPARTICLE();

	public :
		void Init();
		void Link( const MDPARTICLE* );

		void SetMass( float );
		void SetForce( float, float, float );
		void SetVelocity( float, float, float );
		void SetPosition( float, float, float );
		void SetColor( float, float, float, float );

		float GetMass();
		XMFLOAT3 GetForce();
		XMFLOAT3 GetVelocity();
		XMFLOAT3 GetPosition();
		XMFLOAT4 GetColor();

	public :
		MDPARTICLE* m_Connection;

	private :
		float m_MASS;
		XMFLOAT3 m_FORCE;
		XMFLOAT3 m_VELOCITY;
		XMFLOAT3 m_POSITION;
		XMFLOAT4 m_COLOR;
};

#endif