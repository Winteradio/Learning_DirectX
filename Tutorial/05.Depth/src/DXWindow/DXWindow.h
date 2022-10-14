#pragma once

#ifndef __DXWINDOW_H__
#define __DXWINDOW_H__

#include <Windows.h>
#include "Log.h"

class DXWINDOW
{
	// Functions - Constructor and Destructor
	public :

		DXWINDOW();

		DXWINDOW(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nShowCmd );

		~DXWINDOW();

	// Functions - Basic Init and Main Loop
	public :

		bool InitWindow(
			HINSTANCE hInstance,
			int ShowWnd,
			int Width,
			int Height,
			bool Fullscreen );

		void MainLoop();

	private :


	// Variables
	public :

		// Handler for Window
		HWND m_hWnd = NULL;

		// Name of Window Class & Title
		const char* m_WindowName;
		const char* m_WindowTitle;

		// Width and Height of Window
		int m_Width = 800;
		int m_Height = 600;

		// Setting for Fullscreen
		bool m_Fullscreen = false;

	private :
};

// CALLBACK Function for Windows Message
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

#endif