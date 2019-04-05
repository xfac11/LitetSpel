struct GSInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float3 thePoint : MYVALUE;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float3 thePoint : MYVALUE;
};

cbuffer CB_PER_FRAME : register(b0)
{

	float4x4 worldMat;
	float4x4 WorldViewProj;
	//view 
	//projection
	//worldPos 
	float3 camPos;
	float padding;
}

[maxvertexcount(3)]
void GS_main(line GSInput input[2], inout LineStream<GSOutput> theOutput)
{
	GSOutput output;
	for (int i = 0; i < 2; i++)
	{
		output.pos = mul(WorldViewProj, input[i].pos);
		output.tex = input[i].tex;
		output.normal = input[i].normal;
		output.thePoint = input[i].thePoint;
		theOutput.Append(output);
	}
	
	theOutput.RestartStrip();
}