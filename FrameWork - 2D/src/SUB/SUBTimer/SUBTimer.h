#ifndef __SUBTIMER_H__
#define __SUBTIMER_H__

#include "DXCommon.h"

class SUBTIMER
{
	public :

		SUBTIMER();
		SUBTIMER( const SUBTIMER* );
		~SUBTIMER();

	public :

		bool Init();
		void Frame();

		float GetTime();

	private :

		INT64 m_Frequency;
		float m_TicksPerMS;
		INT64 m_StartTime;
		float m_FrameTime;
};

#endif