#ifndef __SUBINPUT_H__
#define __SUBINPUT_H__

#include <Windows.h>
#include <cmath>

struct MOUSEINFO
{
	int PosX = 0;
	int PosY = 0;
	bool LeftButton = false;;
	bool RightButton = false;;
	bool Wheel = false;
};

class SUBINPUT
{
	public :

		SUBINPUT();
		SUBINPUT( const SUBINPUT* );
		~SUBINPUT();

	public :

		bool Init( int ,int );

		MOUSEINFO* GetMouse();
		bool GetRasterizerState();
		LRESULT CALLBACK MessageHandler( HWND, UINT, WPARAM, LPARAM );

	private :

		void SetMousePosition( int, int );
		void SetMouseLeftState( bool );
		void SetMouseRightState( bool );
		void SetMouseWheelState( bool );
		void SetRasterizerState( bool& );

	private :

		unsigned char m_KeyboardState[256];

		int m_ScreenWidth;
		int m_ScreenHeight;
		MOUSEINFO* m_Mouse;
		bool m_WireFrame;
};

#endif