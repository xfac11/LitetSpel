struct PS_IN
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Color : COLOR;
};
struct PS_OUT
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Color : COLOR;
};

Texture2D Tex : register(t0);
SamplerState SampSt :register(s0);

float4 PS_main(PS_IN input) : SV_Target
{

	float4 color = Tex.Sample(SampSt, input.Tex)* float4(input.Color,1.0);
	//return = texture(particleTex, texCoords) * vec4(colorFinal, 1.0);
	return color;// * (input color 1.0)
}