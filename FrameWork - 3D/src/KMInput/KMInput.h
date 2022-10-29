#ifndef __KMINPUT_H__
#define __KMINPUT_H__

#include <Windows.h>
#include <cmath>
#include "Log.h"

struct KMMOUSE
{
	int PosX = 0;
	int PosY = 0;
	bool LeftButton = false;;
	bool RightButton = false;;
	bool Wheel = false;
};

class KMINPUT
{
	public :

		KMINPUT();
		KMINPUT( const KMINPUT* );
		~KMINPUT();

	public :

		bool Init( int ,int );

		KMMOUSE* GetMouse();
		LRESULT CALLBACK MessageHandler( HWND, UINT, WPARAM, LPARAM );

	private :

		void SetMousePosition( int, int );
		void SetMouseLeftState( bool );
		void SetMouseRightState( bool );
		void SetMouseWheelState( bool );

	private :

		unsigned char m_KeyboardState[256];

		int m_ScreenWidth;
		int m_ScreenHeight;
		KMMOUSE* m_Mouse;
};

#endif