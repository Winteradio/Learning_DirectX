// FileName : PSLightColor.hlsl

// Globals
cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

// Typedefs
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

// Pixel Shdaer
float4 ColorPixelShader( PixelInputType input ) : SV_TARGET
{
	float4 Color;
	float3 lightDir;
	float lightIntensity;

	lightDir = -lightDirection;
	lightIntensity = saturate( dot( input.normal, lightDir ) );

	Color = saturate( diffuseColor * lightIntensity );
	Color = input.color * Color;
	return Color;
}