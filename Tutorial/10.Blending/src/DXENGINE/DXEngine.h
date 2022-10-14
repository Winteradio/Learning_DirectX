#pragma once

#ifndef __DXENGINE_H__
#define __DXENGINE_H__

#include "DXWindow.h"
#include "DXCommon.h"
#include "DXVertex.h"
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

		ID3D11VertexShader* m_VS;
		ID3D11PixelShader* m_PS;
		ID3D10Blob* m_VS_Buffer;
		ID3D10Blob* m_PS_Buffer;
		ID3D11InputLayout* m_VertLayout;

		ID3D11Buffer* m_SquareIndexBuffer;
		ID3D11Buffer* m_SquareVertBuffer;

		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11Texture2D* m_DepthStencilBuffer;

		ID3D11RasterizerState* m_WireFrames;

		// Camera
		ID3D11Buffer* m_cbPerObjectBuffer;

		XMMATRIX WVP;
		XMMATRIX World;
		XMMATRIX camView;
		XMMATRIX camProjection;

		XMVECTOR camPosition;
		XMVECTOR camTarget;
		XMVECTOR camUp;

		cbPerObject cbPerObj;
		// Camera

		// Cube Object
		XMMATRIX cube1World;
		XMMATRIX cube2World;

		XMMATRIX Rotation;
		XMMATRIX Scale;
		XMMATRIX Translation;

		float rot = 0.0f;
		// Cube Object

		// Texture
		ID3D11ShaderResourceView* m_CubesTexture;
		ID3D11SamplerState* m_CubesTexSamplerState;

		// Blending
		ID3D11BlendState* m_Transparency;
		ID3D11RasterizerState* m_CCWcullmode;
		ID3D11RasterizerState* m_CWcullmode;

		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;

		int colormodr = 1;
		int colormodg = 1;
		int colormodb = 1;

		const char* VertexShader;
		const char* PixelShader;
		const char* Shader;
		const char* Texture;

	private :

};

#endif