#ifndef __DXENGINE_H__
#define __DXENGINE_H__

#include "DXWindow.h"

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
// #include <xnamath.h> -> warning C4838 >> unsinged int to INT 축소 변환 때문에 관련 이슈

class DXENGINE : public DXWINDOW
{
	// Functions - Constructor and Destructor
	public :

		DXENGINE();

		DXENGINE(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nShowCmd );

		~DXENGINE();


	// Functions - Basic Init
	public :

		bool InitD3D11();

		void MainLoop();

	// Functions - Other
	public :

		void Cleanup();

		bool InitScene();

		bool UpdateScene();

		bool DrawScene();


	// Variables - D3D11 and Color
	public :

		ID3D11Device* m_D3D11Device;
		ID3D11DeviceContext* m_D3D11DevCon;
		ID3D11RenderTargetView* m_RenderTargetView;

		IDXGISwapChain* m_SwapChain;

		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;

		int colormodr = 1;
		int colormodg = 1;
		int colormodb = 1;

	private :

};

#endif