#ifndef __DXENGINE_H__
#define __DXENGINE_H__

#include "DXCommon.h"

#include "DXD3D.h"
#include "DXCamera.h"
#include "DXLight.h"
#include "DXModel.h"
#include "DXText.h"

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

		bool Frame( int, int, float, int, int );
		void Release();

	// Functions - Divide Init Functions
	private :

		void InitFileDIR();

		bool InitDXD3D( int, int, bool, HWND, float, float );
		bool InitDXCAMERA();
		bool InitDXLIGHT( const char*, const char* );
		bool InitDXMODEL( const char*, const char* );
		bool InitDXTEXT( int, int, const char*, const char*, const char*, const char* );

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
		DXTEXT* m_DXTEXT;

		float rotation;
		float move;
		float move_temp;

		const char* m_LVSfileDIR;
		const char* m_LPSfileDIR;
		const char* m_LIMGfileDIR;

		const char* m_MDfileDIR;

		const char* m_TVSfileDIR;
		const char* m_TPSfileDIR;
		const char* m_TFontfileDIR;
		const char* m_TDDSfileDIR;
};

#endif __DXENGINE_H__