struct GSInput
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 Normal : NORMAL;
};
struct GSOutput
{
	float4 Pos: SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 wPosition : POSITION;
	float4 NormalWS : NORMAL;
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
[maxvertexcount(3)]
void GS_main(triangle GSInput input[3], inout TriangleStream<GSOutput> theOutput)
{

	GSOutput output;
	float3 pos0 = mul(float4(input[0].Pos.xyz, 1.0f), world).xyz;
	float3 pos1 = mul(float4(input[1].Pos.xyz, 1.0f), world).xyz;
	float3 pos2 = mul(float4(input[2].Pos.xyz, 1.0f), world).xyz;
	float3 u = pos1 - pos0;
	float3 v = pos2 - pos0;; //pos of 3 == 0
	float3 normal = cross(u, v);
	normal = normalize(normal);
	for (int i = 0; i < 3; i++)
	{
		//output.Pos = mul(world, input[i].Pos);
		//output.Pos = mul(view, output.Pos);
		output.Pos = mul(input[i].Pos, world);
		output.Pos = mul(output.Pos, view);
		output.Pos = mul(output.Pos, proj);
		//output.Pos = mul(proj, input[i].Pos);
		//output.Pos = input[i].Pos;
		output.wPosition = mul(float4(input[i].Pos.xyz, 1.0f), world);
		output.Tex = input[i].Tex;
		output.NormalWS = float4(normal,1.0f);
		//output.NormalWS = mul(float4(normal,0.0f),world);
		theOutput.Append(output);
	}
}
