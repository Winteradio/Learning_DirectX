#ifndef __DXCAMERA_H__
#define __DXCAMERA_H__

#include "DXCommon.h"

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
		bool Frame( bool, int, int, int, int, int, int, int, float, float );

	// Functions - Set and Get
	public :

		void SetPosition( float, float, float );
		void SetRotation( float, float, float );
		void SetLookAt( float, float, float );
		void SetLookUp( float, float, float );

		void GetViewMatrix( XMMATRIX& );
		void GetOrthoMatrix( XMMATRIX& );
		XMFLOAT3 GetPosition();
		XMFLOAT3 GetRotation();
		XMFLOAT3 GetLookAt();
		XMFLOAT3 GetLookUp();

	// Variables
	private :

		XMFLOAT3 m_Position;
		XMFLOAT3 m_Rotation;
		XMFLOAT3 m_DirFocus;
		XMFLOAT3 m_DirUp;
		XMMATRIX m_ViewMatrix;
		XMMATRIX m_OrthoMatrix;
		XMFLOAT2 m_OrthoPos;
};


#endif