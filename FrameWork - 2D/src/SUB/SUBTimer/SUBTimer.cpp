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

	m_StateTime = timeGetTime();

	return true;
}

void SUBTIMER::Frame()
{
	m_FinishTime = timeGetTime();
	m_Time = ( m_FinishTime - m_StateTime ) / 1000.0f;
}

float SUBTIMER::GetTime()
{
	return m_Time;
}