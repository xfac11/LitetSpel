struct GSInput
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
};

struct GSOutput
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float3 wPosition : POSITION;
};

cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 world;//world
	float4x4 view;//view
	float4x4 proj;//proj
	float3 camPos;
	float padding;
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
		output.Pos = mul(proj, input[i].Pos);
		output.wPosition = mul(float4(input[i].Pos.xyz, 1.0f), world).xyz;
		output.Tex = input[i].Tex;
		output.Normal = normal;
		//output.normal = mul((float3x3)worldMat, normal);
		theOutput.Append(output);
	}
	
}

