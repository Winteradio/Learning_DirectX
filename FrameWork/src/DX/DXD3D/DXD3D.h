#ifndef __DXD3D_H__
#define __DXD3D_H__

#include "DXCommon.h"

class DXD3D
{

	// Functions - Constructor and Destructor
	public :

		DXD3D();
		DXD3D( const DXD3D* );
		~DXD3D();

	// Functions - Basic Initialization, End and Action related Scene
	public :

		bool Init( int, int, bool, HWND, float, float );
		void Release();

		void BeginScene( float, float, float, float );
		void EndScene();

	// Functions - Divide Init Functions
	private :

		bool InitSwapChain( int, int, HWND );
		bool InitRenderTarget();
		bool InitDepthStencil( int, int );
		bool InitRasterizer();
		void InitViewport( int, int );
		void InitMatrix( int, int, float, float );
		bool InitBlend();

		void InitPointer();

	// Functions - Get private variables
	public :
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		void GetProjectionMatrix( XMMATRIX& );
		void GetWorldMatrix( XMMATRIX& );
		void GetOrthoMatrix( XMMATRIX& );

		void TurnWireFrameOn();
		void TurnWireFrameOff();

		void TurnZBufferOn();
		void TurnZBufferOff();

		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();


	// Variables - Direct3D Device
	private :

		IDXGISwapChain* m_SwapChain;

		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		ID3D11RenderTargetView* m_RenderTargetView;

		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilState* m_DepthEnabledStencilState;
		ID3D11DepthStencilState* m_DepthDisabledStencilState;
		ID3D11DepthStencilView* m_DepthStencilView;

		ID3D11RasterizerState* m_RasterStateSL;
		ID3D11RasterizerState* m_RasterStateWF;

		ID3D11BlendState* m_AlphaEnabledBlendingState;
		ID3D11BlendState* m_AlphaDisabledBlendingState;

		XMMATRIX m_ProjectionMatrix;
		XMMATRIX m_WorldMatrix;
		XMMATRIX m_OrthoMatrix;
};

#endif __DXD3D_H__