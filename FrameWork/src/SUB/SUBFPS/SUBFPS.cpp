#include "SUBFPS.h"

SUBFPS::SUBFPS() {}

SUBFPS::SUBFPS( const SUBFPS* Other )
{
	*this = *Other;
}

SUBFPS::~SUBFPS() {}

bool SUBFPS::Init()
{
	m_FPS = 0;
	m_Count = 0;
	m_StartTime = timeGetTime();

	return true;
}

void SUBFPS::Frame()
{
	m_Count++;

	if ( timeGetTime() >= ( m_StartTime + 1000 ) )
	{
		m_FPS = m_Count;
		m_Count = 0;

		m_StartTime = timeGetTime();
	}
}

int SUBFPS::GetFPS()
{
	return m_FPS;
}