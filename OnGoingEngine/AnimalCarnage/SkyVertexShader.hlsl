cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 view;//view
	float4x4 proj;//proj
};

cbuffer CB_WORLD : register(b1)
{
	float4x4 world;//world
};
struct VS_IN
{
	float4 position : POSITION;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Tex : TEXCOORD;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	output.Pos = mul(input.position, world);
	output.Pos = mul(output.Pos, view);
	output.Pos = mul(output.Pos, proj).xyww;
	//output.Pos = mul(proj, input.position).xyww;//proj is worldviewproj, xyww because we want to set z to 1.0f so it is the furthest object always
	output.Tex = input.position.xyz;
	return output;
}