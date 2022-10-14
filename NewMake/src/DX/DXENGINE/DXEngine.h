#ifndef __DXENGINE_H__
#define __DXENGINE_H__

#include "DXCommon.h"

#include "DXD3D.h"
#include "Log.h"

// Variables - Globals
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 1.0f;



class DXENGINE
{
	// Functions - Constructor and Destructor
	public :

		DXENGINE();
		DXENGINE( const DXENGINE& );
		~DXENGINE();

	// Functions - Basic Initialization and Run Frame & End
	public :

		bool Init( int, int, HWND );

		bool Frame();
		void Done();

	// Functions - Render for each frames
	private :

		bool Render();

	// Variables - DXD3D Object
	private :

		DXD3D* m_DXD3D = nullptr;
};

#endif __DXENGINE_H__