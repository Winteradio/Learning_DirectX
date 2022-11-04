#include "SUBInput.h"
#include "SUBLog.h"

SUBINPUT::SUBINPUT() {}

SUBINPUT::SUBINPUT( const SUBINPUT* Other )
{
	*this = *Other;
}

SUBINPUT::~SUBINPUT() {}

bool SUBINPUT::Init( int screenWidth,int screenHeight )
{
	m_Mouse = new MOUSEINFO;

	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;
	m_Mouse->ScreenWidth = m_ScreenWidth;
	m_Mouse->ScreenHeight = m_ScreenHeight;

	m_WireFrame = false;

	return true;
}

MOUSEINFO* SUBINPUT::GetMouse() { return m_Mouse; }
bool SUBINPUT::GetRasterizerState() { return m_WireFrame; }


LRESULT CALLBACK SUBINPUT::MessageHandler( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	POINTS Point;

	switch ( message )
	{
		case WM_KEYDOWN :
		{
			switch ( wParam )
			{
				case VK_ESCAPE :
				{
					LOG_INFO(" Pushed Exit Key \n ");
					DestroyWindow( hWnd );

					return 0;
				}

				case VK_F1 :
				{
					SetRasterizerState( m_WireFrame );

					return 0;
				}
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
		}

		case WM_MOUSEWHEEL :
		{
			Point.x = GET_WHEEL_DELTA_WPARAM( wParam );
			if ( m_Mouse->Wheel )
			{
				SetMouseWheelDir( Point.x );
			}
			else
			{
				SetMouseWheelDir( 0 );
			}
			return 0;
		}

		case WM_LBUTTONUP :
		{
			SetMouseLeftState( false );
			return 0;
		}

		case WM_RBUTTONUP :
		{
			SetMouseRightState( false );
			return 0;
		}

		case WM_MBUTTONDOWN :
		{
			SetMouseWheelState( m_Mouse->Wheel );
			return 0;
		}

		default :
		{
			return DefWindowProc( hWnd, message, wParam, lParam );
		}
	}
}

void SUBINPUT::SetMousePosition( int mouseX, int mouseY )
{
	m_Mouse->PrevPosX = m_Mouse->PosX;
	m_Mouse->PrevPosY = m_Mouse->PosY;

	m_Mouse->PosX = mouseX - m_ScreenWidth / 2;
	m_Mouse->PosY = (-1) * ( mouseY - m_ScreenHeight / 2 );

	if ( abs( m_Mouse->PosX - m_Mouse->PrevPosX ) <= 1.0f && abs( m_Mouse->PosY - m_Mouse->PrevPosY ) <= 1.0f )
	{
		m_Mouse->PrevPosX = m_Mouse->PosX;
		m_Mouse->PrevPosY = m_Mouse->PosY;
	}
}

void SUBINPUT::SetMouseWheelDir( int WheelDir )
{
	m_Mouse->WheelDir = WheelDir / 120;
}


void SUBINPUT::SetMouseLeftState( bool buttonState ) { m_Mouse->LeftButton = buttonState; }
void SUBINPUT::SetMouseRightState( bool buttonState ) { m_Mouse->RightButton = buttonState; }
void SUBINPUT::SetMouseWheelState( bool& buttonState )
{
	if ( buttonState )
	{
		buttonState = false;
	}
	else
	{
		buttonState = true;
	}
}

void SUBINPUT::SetRasterizerState( bool& RasterizerState )
{
	if ( RasterizerState == true )
	{
		LOG_INFO(" Turn off Wire Frame \n ");
		RasterizerState = false;
	}
	else
	{
		LOG_INFO(" Turn on Wire Frame \n ");
		RasterizerState = true;
	}
}