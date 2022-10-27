// FileName : Color.hlsl

// Globals

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
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
	float3 normal : NORMAL0;
	float3 viewDirection : NORMAL1;
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

	float4 worldPosition = mul( input.position, worldMatrix );
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	output.viewDirection = normalize( output.viewDirection );

	return output;
};