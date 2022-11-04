#ifndef __DXL_SHADER_H__
#define __DXL_SHADER_H__

#include "DXCommon.h"

#define DXLTEXTURE 0
#define DXLCOLOR 1

class DXL_SHADER
{
	// Structor - MatrixBufferType and LightBufferType
	private:

		struct MatrixBufferType
		{
			XMMATRIX world;
			XMMATRIX view;
			XMMATRIX projection;
		};

		struct CameraBufferType
		{
			XMFLOAT3 cameraPosition;
			float padding;
		};

		struct LightBufferType
		{
			XMFLOAT4 diffuseColor;
			XMFLOAT3 lightDirection;
			float padding;
		};

	// Functions - Constructor and Destructor
	public :

		DXL_SHADER();
		DXL_SHADER( const DXL_SHADER* );
		~DXL_SHADER();

	// Functions - Init, Release Object and Render
	public :

		bool Init( ID3D11Device*, ID3D11DeviceContext*, const char*, const char* );
		void Release();

		bool Render( ID3D11DeviceContext*, int,
			XMMATRIX, XMMATRIX, XMMATRIX,
			ID3D11ShaderResourceView*,
			XMFLOAT3, XMFLOAT4, XMFLOAT3 );


	// Functions - Render and etc...
	private :

		bool SetShaderParameters( ID3D11DeviceContext*,
			XMMATRIX, XMMATRIX, XMMATRIX,
			ID3D11ShaderResourceView*,
			XMFLOAT3, XMFLOAT4, XMFLOAT3 );

		void ShaderErrorMessage( ID3D10Blob* );


	// Functions - Divide Init Functions
	private :

		bool InitVertexShader( ID3D11Device*, ID3D11DeviceContext*, const char* );
		bool InitPixelShader( ID3D11Device*, ID3D11DeviceContext*, const char* );
		bool InitLayout( ID3D11Device* );
		bool InitSampleState( ID3D11Device* );
		bool InitMatrixBuffer( ID3D11Device* );
		bool InitLightBuffer( ID3D11Device* );
		bool InitCameraBuffer( ID3D11Device * );

		void InitPointer();


	// Variables - Shader & Matrix Buffer and Layout, Shader File Directory
	private:

		int m_ShaderSet;

		ID3D10Blob* m_VertexShaderBuffer;
		ID3D11VertexShader* m_VertexShader;

		ID3D10Blob* m_PixelShaderBuffer;
		ID3D11PixelShader* m_PixelShader;

		ID3D11InputLayout* m_Layout;

		ID3D11SamplerState* m_SamplerState;

		ID3D11Buffer* m_MatrixBuffer;
		ID3D11Buffer* m_LightBuffer;
		ID3D11Buffer* m_CameraBuffer;
};


#endif