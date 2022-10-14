#include "DXEngine.h"

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{

	DXENGINE* NewDirect = new DXENGINE( hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	NewDirect->MainLoop();

	system("pause");

	// true / fale : 1 / 0
	// SUCCEEDED / FAILED : 0 / 1
	return 0;
}