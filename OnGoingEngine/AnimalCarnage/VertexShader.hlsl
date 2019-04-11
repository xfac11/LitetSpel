struct VS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 Normal : NORMAL;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 Normal : NORMAL;
};

cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 view;//view
	float4x4 proj;//proj
	float4 camPos;
}

cbuffer world : register(b1)
{
	float4x4 world;//world
}
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;


	output.Pos = float4(input.Pos.xyz, 1.0);
	//output.WorldPos = mul((float3x3)worldMat, input.Pos.xyz);

	//output.Color = input.Color;
	output.Tex = input.Tex;
	output.Normal = float4(input.Normal.xyz,1.0f);
	return output;
}