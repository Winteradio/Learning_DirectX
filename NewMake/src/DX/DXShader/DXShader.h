#ifndef __DXSHADER_H__
#define __DXSHADER_H__

#include "DXCommon.h"

class DXSHADER
{
	// Structor - MatrixBufferType
	private:

		struct MatrixBufferType
		{
			XMMATRIX world;
			XMMATRIX view;
			XMMATRIX projection;
		};

	// Functions - Constructor and Destructor
	public :

		DXSHADER();
		DXSHADER( const DXSHADER* );
		~DXSHADER();

	// Functions - Init, Release Object and Render
	public :

		bool Init( ID3D11Device* );
		void Release();

		bool Render( ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX );


	// Functions - Render and etc...
	private :

		bool SetShaderParameters( ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX );

		void ShaderErrorMessage( ID3D10Blob* );


	// Functions - Divide Init Functions
	private :

		void InitShaderDIR();
		bool InitVertexShader( ID3D11Device* );
		bool InitPixelShader( ID3D11Device* );
		bool InitLayout( ID3D11Device* );
		bool InitMatrixBuffer( ID3D11Device* );

		void InitPointer();


	// Variables - Shader & Matrix Buffer and Layout, Shader File Directory
	private:

		ID3D10Blob* m_VertexShaderBuffer;
		ID3D11VertexShader* m_VertexShader;
		ID3D10Blob* m_PixelShaderBuffer;
		ID3D11PixelShader* m_PixelShader;
		ID3D11InputLayout* m_Layout;
		ID3D11Buffer* m_MatrixBuffer;

		const char* m_VSfile;
		const char* m_PSfile;
};


#endif