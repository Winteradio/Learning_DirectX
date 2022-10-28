// FileName : PixelShader.hlsl

// Globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
};

// Typedefs
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 viewDirection : TEXCOORD1;
};

// Pixel Shdaer
float4 ColorPixelShader( PixelInputType input ) : SV_TARGET
{
	float4 textureColor = shaderTexture.Sample( SampleType, input.tex );

	float4 color = ambientColor;

	float4 specular = float4( 0.0f, 0.0f, 0.0f, 0.0f );

	float3 lightDir = -lightDirection;

	float lightIntensity = saturate( dot( input.normal, lightDir ) );

	if ( lightIntensity > 0.0f )
	{
		color += ( diffuseColor * lightIntensity );

		color = saturate( color );

		float3 reflection = normalize( 2 * lightIntensity * input.normal - lightDir );

		specular = pow( saturate( dot ( reflection, input.viewDirection ) ), specularPower
			 );
	}

	color = color * textureColor;

	color = saturate( color + specular );
	return color;
};