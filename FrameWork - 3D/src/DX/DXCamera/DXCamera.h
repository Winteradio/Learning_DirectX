#ifndef __DXCAMERA_H__
#define __DXCAMERA_H__

#include "DXCommon.h"

#define PI 3.14159265358979323846

class DXCAMERA
{
	// Functions - Constructor and Destructor
	public :

		DXCAMERA();
		DXCAMERA( const DXCAMERA* );
		~DXCAMERA();

	// Function - Render
	public :

		void Render();
		bool Frame( int, int );

	// Functions - Set and Get
	public :

		void SetPosition( float, float, float );
		void SetRotation( float, float, float );

		void GetViewMatrix( XMMATRIX& );
		XMFLOAT3 GetPosition();
		XMFLOAT3 GetRotation();

	// Variables
	private :

		XMFLOAT3 m_Position;
		XMFLOAT3 m_Rotation;
		XMMATRIX m_ViewMatrix;
};


#endif