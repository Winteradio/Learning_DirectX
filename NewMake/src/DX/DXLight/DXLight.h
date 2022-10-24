#ifndef __DXLIGHT_H__
#define __DXLIGHT_H__

#include "DXCommon.h"

class DXLIGHT
{
	// Functions - Constructor and Destructor
	public :

		DXLIGHT();
		DXLIGHT( const DXLIGHT* );
		~DXLIGHT();

	// Functions - Set and Get Light information
	public :

		void SetAmbientColor( float, float, float, float );
		void SetDiffuseColor( float, float, float ,float );
		void SetDirection( float, float, float );

		XMFLOAT4 GetAmbientColor();
		XMFLOAT4 GetDiffuseColor();
		XMFLOAT3 GetDirection();

	// Variables - Setting Value for Light
	private :

		XMFLOAT4 m_AmbientColor;
		XMFLOAT4 m_DiffuseColor;
		XMFLOAT3 m_Direction;
};

#endif
