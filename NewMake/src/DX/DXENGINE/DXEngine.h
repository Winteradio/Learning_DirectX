#ifndef __DXENGINE_H__
#define __DXENGINE_H__

#include "DXCommon.h"

#include "DXD3D.h"
#include "DXCamera.h"
#include "DXLight.h"
#include "DXModel.h"
#include "DXShader.h"

// Variables - Globals
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 1.0f;



class DXENGINE
{
	// Functions - Constructor and Destructor
	public :

		DXENGINE();
		DXENGINE( const DXENGINE* );
		~DXENGINE();

	// Functions - Basic Initialization and Run Frame & End
	public :

		bool Init( int, int, HWND );

		bool Frame();
		void Release();

	// Functions - Divide Init Functions
	private :

		bool InitDXD3D( int, int, bool, HWND, float, float );
		bool InitDXCAMERA();
		bool InitDXLIGHT();
		bool InitDXMODEL();
		bool InitDXSHADER();

	// Functions - Render for each frames
	private :

		bool Render( float );

		void InitPointer();

	// Variables - DXD3D Object
	private :

		DXD3D* m_DXD3D;
		DXCAMERA* m_DXCAMERA;
		DXLIGHT* m_DXLIGHT;
		DXMODEL* m_DXMODEL;
		DXSHADER* m_DXSHADER;

		float rotation;
};

#endif __DXENGINE_H__