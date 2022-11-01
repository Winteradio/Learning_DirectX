// FileName : VSLightColor.hlsl

// Globals

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// Typedefs
struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

// Vertex Shader
PixelInputType ColorVertexShader( VertexInputType input )
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul( input.position, worldMatrix );
	output.position = mul( output.position, viewMatrix );
	output.position = mul( output.position, projectionMatrix );

	output.normal = mul( input.normal, (float3x3)worldMatrix );
	output.normal = normalize( output.normal );
	output.color = input.color;

	return output;
}