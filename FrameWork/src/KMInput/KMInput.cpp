#include "KMInput.h"

KMINPUT::KMINPUT(){}

KMINPUT::KMINPUT( const KMINPUT* Other )
{
	*this = *Other;
}

KMINPUT::~KMINPUT() {}

bool KMINPUT::Init( int screenWidth,int screenHeight )
{
	m_Mouse = new KMMOUSE;

	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	return true;
}

KMMOUSE* KMINPUT::GetMouse()
{
	return m_Mouse;
}

LRESULT CALLBACK KMINPUT::MessageHandler( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	POINTS Point;

	switch ( message )
	{
		case WM_KEYDOWN :
		{
			if ( wParam == VK_ESCAPE )
			{
				LOG_INFO("Pushed Exit Key \n ");
				DestroyWindow( hWnd );
				return 0;
			}
		}

		case WM_DESTROY :
		{
			PostQuitMessage( 0 );
			return 0;
		}

		case WM_MOUSEMOVE :
		{
			Point.x = LOWORD( lParam );
			Point.y = HIWORD( lParam );
			SetMousePosition( Point.x, Point.y );

			if ( wParam == MK_LBUTTON )
			{
				SetMouseLeftState( true );
				return 0;
			}
			else if ( wParam == MK_RBUTTON )
			{
				SetMouseRightState( true );
				return 0;
			}
			else if ( wParam == MK_MBUTTON )
			{
				SetMouseWheelState( true );
				return 0;
			}
		}

		case WM_LBUTTONUP :
		{
			SetMouseLeftState( false );
			return 0;
		}

		case WM_RBUTTONUP :
		{
			SetMouseRightState( false );
		}

		case WM_MBUTTONUP :
		{
			SetMouseWheelState( false );
		}

		default :
		{
			return DefWindowProc( hWnd, message, wParam, lParam );
		}
	}
}

void KMINPUT::SetMousePosition( int mouseX, int mouseY )
{
	m_Mouse->PosX = mouseX - m_ScreenWidth / 2;
	m_Mouse->PosY = (-1) * ( mouseY - m_ScreenHeight / 2 );
}


void KMINPUT::SetMouseLeftState( bool buttonState ) { m_Mouse->LeftButton = buttonState; }
void KMINPUT::SetMouseRightState( bool buttonState ) { m_Mouse->RightButton = buttonState; }
void KMINPUT::SetMouseWheelState( bool buttonState ) { m_Mouse->Wheel = buttonState; }