#ifndef __MAIN_CPP__
#define __MAIN_CPP__

#include "System.h"

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	SYSTEM* Wnd = new SYSTEM;
	if (Wnd)
	{
		Wnd->Run();
	}

	Wnd->Release();
	delete Wnd;
	Wnd = nullptr;

	system("pause");
	return 0;
}

#endif