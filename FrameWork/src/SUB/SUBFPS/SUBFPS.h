#ifndef __SUBFPS_H__
#define __SUBFPS_H__

#include <Windows.h>
#include <mmsystem.h>

class SUBFPS
{
	public :

		SUBFPS();
		SUBFPS( const SUBFPS* );
		~SUBFPS();

	public :

		bool Init();
		void Frame();
		int GetFPS();

	private :

		int m_FPS;
		int m_Count;
		unsigned long m_StartTime;
};

#endif