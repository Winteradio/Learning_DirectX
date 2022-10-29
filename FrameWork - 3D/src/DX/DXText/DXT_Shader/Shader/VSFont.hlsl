// Filename : VSFont.hlsl

// Globals
cbuffer ConstantBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// TypeDefs
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// Vertex Shader
PixelInputType FontVertexShader( VertexInputType input )
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul( input.position, worldMatrix );
	output.position = mul( output.position, viewMatrix );
	output.position = mul( output.position, projectionMatrix );

	output.tex = input.tex;

	return output;
}
