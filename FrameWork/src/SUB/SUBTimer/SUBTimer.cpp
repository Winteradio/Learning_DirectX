#include "SUBTimer.h"

SUBTIMER::SUBTIMER() {}

SUBTIMER::SUBTIMER( const SUBTIMER* Other )
{
	*this = *Other;
}

SUBTIMER::~SUBTIMER() {}

bool SUBTIMER::Init()
{
	QueryPerformanceFrequency( ( LARGE_INTEGER*)&m_Frequency );
	if ( m_Frequency == 0 )
	{
		LOG_ERROR(" Failed - System has High Quality Timer \n ");
		return false;
	}

	m_TicksPerMS = (float)( m_Frequency / 1000 );
	QueryPerformanceCounter( ( LARGE_INTEGER*)&m_StartTime );

	return true;
}

void SUBTIMER::Frame()
{
	INT64 currentTime = 0;

	QueryPerformanceCounter( ( LARGE_INTEGER*)&currentTime );
	float timeDifference = (float)( currentTime - m_StartTime );

	m_FrameTime = timeDifference / m_TicksPerMS;
	m_StartTime = currentTime;
}

float SUBTIMER::GetTime()
{
	return m_FrameTime;
}