#ifndef __DXT_SHADER_H__
#define __DXT_SHADER_H__

#include "DXCommon.h"

class DXT_SHADER
{
	private :

		struct ConstantBufferType
		{
			XMMATRIX world;
			XMMATRIX view;
			XMMATRIX projection;
		};

		struct PixelBufferType
		{
			XMFLOAT4 pixelColor;
		};

	public :

		DXT_SHADER();
		DXT_SHADER( const DXT_SHADER* );
		~DXT_SHADER();

	public :

		bool Init( ID3D11Device*, ID3D11DeviceContext*, const char*, const char*  );
		void Release();
		bool Render( ID3D11DeviceContext*, int,
			XMMATRIX, XMMATRIX, XMMATRIX,
			ID3D11ShaderResourceView*, XMFLOAT4 );

	private :

		bool SetShaderParameters( ID3D11DeviceContext*,
			XMMATRIX, XMMATRIX, XMMATRIX,
			ID3D11ShaderResourceView*, XMFLOAT4 );
		void ShaderErrorMessage( ID3D10Blob* );


	private :

		bool InitVertexShader( ID3D11Device*, ID3D11DeviceContext*, const char*  );
		bool InitPixelShader( ID3D11Device*, ID3D11DeviceContext*, const char*  );
		bool InitLayout( ID3D11Device* );
		bool InitSampleState( ID3D11Device* );
		bool InitConstantBuffer( ID3D11Device* );
		bool InitPixelBuffer( ID3D11Device* );

		void InitPointer();

	private :

		ID3D10Blob* m_VertexShaderBuffer;
		ID3D11VertexShader* m_VertexShader;

		ID3D10Blob* m_PixelShaderBuffer;
		ID3D11PixelShader* m_PixelShader;

		ID3D11InputLayout* m_Layout;
		ID3D11SamplerState* m_SamplerState;
		ID3D11Buffer* m_ConstantBuffer;
		ID3D11Buffer* m_PixelBuffer;
};

#endif