#include "System.h"
#include "Log.h"

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	SYSTEM* Wnd = new SYSTEM;
	if (Wnd)
	{
		Wnd->Run();
	}

	Wnd->Done();
	delete Wnd;
	Wnd = nullptr;

	system("pause");
	return 0;
}