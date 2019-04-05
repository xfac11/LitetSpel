struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
};
Texture2D Tex:register(t0);
SamplerState SampSt :register(s0);
float4 PS_main(VS_OUT input) : SV_Target
{
	return float4(input.Color,1.f);
}