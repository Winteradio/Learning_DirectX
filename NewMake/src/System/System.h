#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <Windows.h>

#include "DXEngine.h"
#include "Log.h"


class SYSTEM
{
	// Functions - Constructor and Destructor
	public :

		SYSTEM();
		SYSTEM( const SYSTEM* );
		~SYSTEM();

	// Functions - Running and Ending(Done)
	public :

		void Run();
		void Release();

	// Functions - Basic Initialization and Run Frame
	private :

		bool Init();
		bool Frame();
		bool InitWindows();
		bool InitDXENGINE();

		void InitPointer();

	// Variables
	public :

		// Handler for Window
		HWND m_hWnd;

		// Instance for Window
		HINSTANCE m_hInstance;

		// Name of Window Class & Title
		const char* m_WindowName;
		const char* m_WindowTitle;

		// Width and Height of Window
		int m_Width;
		int m_Height;

		// Setting for FullScreen
		bool m_FullScreen;

		DXENGINE* m_DXENGINE;
};

static LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

static SYSTEM* MSGHandle = 0;

#endif __SYSTEM_H__