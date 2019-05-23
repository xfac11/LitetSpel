struct GSOutput
{
	float4 Pos: SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState smpl : register(s0);

float4 PS_main(GSOutput input) : SV_TARGET
{
	return tex.Sample(smpl, input.uv);
}
