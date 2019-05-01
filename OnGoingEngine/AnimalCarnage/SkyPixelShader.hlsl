struct PS_IN
{
	float4 Pos : SV_POSITION;
	float3 Tex : TEXCOORD;
};

TextureCube Tex : register(t0);
SamplerState SkyBoxSampler:register(s0);

float4 PS_main(PS_IN input) : SV_Target
{
	return float4(Tex.Sample(SkyBoxSampler, input.Tex).xyz,1.0f);
}