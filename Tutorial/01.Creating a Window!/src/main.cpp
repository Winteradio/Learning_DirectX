#include "DXEngine.h"

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{

	DXENGINE* NewDirect = new DXENGINE( hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	NewDirect->MainLoop();

	return 0;
}