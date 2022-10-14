#ifndef __DXD3D_H__
#define __DXD3D_H__

#include "DXCommon.h"

class DXD3D
{
	// Functions - Constructor and Destructor
	public :

		DXD3D();
		DXD3D( const DXD3D& );
		~DXD3D();

	// Functions - Basic Initialization, End and Action related Scene
	public :

		bool Init( int, int, bool, HWND, float, float );
		void Done();

		void BeginScene( float, float, float, float );
		void EndScene();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		void GetProjectionMatrix( XMMATRIX& );
		void GetWorldMatrix( XMMATRIX& );
		void GetOrthoMatrix( XMMATRIX& );

		void GetVideoCardInfo( char*, int& );

	// Functions - Divide Init Functions
	private :

	// Variables - Direct3D Device
	private :

		bool m_VSYNC_ENABLED = false;

		int m_VideoCardMemory = 0;
		char m_VideoCardDesription[128] = {0};

		IDXGISwapChain* m_SwapChain = nullptr;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11Texture2D* m_Texture2D = nullptr;
		ID3D11DepthStencilState* m_DepthStencilState = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		ID3D11RasterizerState* m_RasterState = nullptr;

		XMMATRIX m_ProjectionMatrix;
		XMMATRIX m_WorldMatrix;
		XMMATRIX m_OrthoMatrix;
};
// Class 종료시 ;를 삽입하지 않을 시
// Error
// error C2533: 'DXD3D::{ctor}' : 생성자에서 반환 형식을 사용할 수 없습니다.

#endif __DXD3D_H__