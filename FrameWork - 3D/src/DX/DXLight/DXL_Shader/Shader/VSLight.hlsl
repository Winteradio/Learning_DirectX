// FileName : Color.hlsl

// Globals

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
	float3 cameraPosition;
	float padding;
};

cbuffer OutputBuffer : register(b2)
{
	float4 Out_POS;
	float3 Out_NOR;
};

// Typedefs
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

// Vertex Shader
PixelInputType ColorVertexShader( VertexInputType input )
{
	PixelInputType output = (PixelInputType)0;

	input.position.w = 1.0f;

	output.position = mul( input.position, worldMatrix );
	output.position = mul( output.position, viewMatrix );
	output.position = mul( output.position, projectionMatrix );

	output.tex = input.tex;

	output.normal = mul( input.normal, (float3x3)worldMatrix );
	output.normal = normalize( output.normal );

	float4 worldPosition = mul( input.position, worldMatrix );

	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	output.viewDirection = normalize( output.viewDirection );

	Out_POS = output.position;
	Out_NOR = output.normal;

	return output;
};
