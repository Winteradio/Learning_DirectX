// FileName : PixelShader.hlsl

// Globals
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
	float4 color : COLOR;
	float3 normal : NORMAL0;
	float3 viewDirection : NORMAL1;
};

// Pixel Shdaer
float4 ColorPixelShader( PixelInputType input ) : SV_TARGET
{
	float4 Color = ambientColor;
	float3 lightDir = - lightDirection;
	float4 specular = float4( 0.0f, 0.0f, 0.0f, 0.0f );

	float lightIntensity = saturate( dot( input.normal, lightDir ) );

	if ( lightIntensity > 0.0f )
	{
		Color += ( diffuseColor * lightIntensity );

		Color = saturate( Color );

		float3 reflection = normalize( 2 * lightIntensity * input.normal - lightDir );

		specular = pow( saturate ( dot ( reflection, input.viewDirection )), specularPower );
	}
	Color = input.color * Color;
	Color = Color + specular;
	return Color;
};