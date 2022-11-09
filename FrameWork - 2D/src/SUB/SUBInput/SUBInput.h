#ifndef __SUBINPUT_H__
#define __SUBINPUT_H__

#include <Windows.h>
#include <cmath>

struct MOUSEINFO
{
	int ScreenWidth = 0;
	int ScreenHeight = 0;
	int PosX = 0;
	int PosY = 0;
	int PrevPosX = 0;
	int PrevPosY = 0;
	bool LeftButton = false;;
	bool RightButton = false;;
	bool Wheel = false;
	int WheelDir = 0;
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
		bool GetInsertState();
		LRESULT CALLBACK MessageHandler( HWND, UINT, WPARAM, LPARAM );

	private :

		void SetMousePosition( int, int );
		void SetMouseWheelDir( int );
		void SetMouseLeftState( bool );
		void SetMouseRightState( bool );
		void SetMouseWheelState( bool );
		void SetRasterizerState( bool& );
		void SetInsertState( bool& );

	private :

		unsigned char m_KeyboardState[256];

		int m_ScreenWidth;
		int m_ScreenHeight;
		MOUSEINFO* m_Mouse;
		bool m_WireFrame;
		bool m_Insert;
};

#endif