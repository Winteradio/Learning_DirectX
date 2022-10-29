#include "SUBCPU.h"
#include "SUBLog.h"

SUBCPU::SUBCPU() {}

SUBCPU::SUBCPU( const SUBCPU* Other )
{
	*this = *Other;
}

SUBCPU::~SUBCPU() {}

void SUBCPU::Init()
{
	m_CanReadCPU = true;
	m_LastSampleTime = 0;
	m_CPUUsage = 0;

	PDH_STATUS status = PdhOpenQuery( NULL, 0, &m_QueryHandle );
	if ( status != ERROR_SUCCESS )
	{
		LOG_ERROR(" Failed - Read CPU \n ");
		m_CanReadCPU = false;
	}

	status = PdhAddCounter( m_QueryHandle, TEXT("Processor(_Total)\\% processor time"), 0, &m_CounterHandle );
	if ( status != ERROR_SUCCESS )
	{
		LOG_ERROR(" Failed - Read CPU \n ");
		m_CanReadCPU = false;
	}

	m_LastSampleTime = GetTickCount();

}

void SUBCPU::Release()
{
	if ( m_CanReadCPU )
	{
		PdhCloseQuery( m_QueryHandle );
	}
}

void SUBCPU::Frame()
{
	PDH_FMT_COUNTERVALUE value;

	if ( m_CanReadCPU )
	{
		if ( ( m_LastSampleTime + 1000 ) < GetTickCount() )
		{
			m_LastSampleTime = GetTickCount();
			PdhCollectQueryData( m_QueryHandle );
			PdhGetFormattedCounterValue( m_CounterHandle, PDH_FMT_LONG, NULL, &value );
			m_CPUUsage = value.longValue;
		}
	}
}

int SUBCPU::GetCPUPercent()
{
	int usage = 0;
	if ( m_CanReadCPU )
	{
		usage = (int)m_CPUUsage;
	}

	return usage;
}