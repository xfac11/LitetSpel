struct GSInput
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Color : COLOR;
};
struct GSOutput
{
	//optimal layot should be
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Color : COLOR;
};
cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 world;//world
	float4x4 view;//view
	float4x4 proj;//proj
	float3 camPos;
	float padding;
};
[maxvertexcount(4)]
void GS_main(point GSInput input[1], inout TriangleStream<GSOutput> theOutput)
{
	//uv
	float2 offsets[4] = {
		0.f, 0.f, //top left
		1.f, 0.f, //top right
		0.f, 1.f,  //bottom left
		1.f, 1.f //bottom right
	};
	//float2 offsets[6] = 
	//{	
	//	0.f, 1.f,	//bottom left
	//	0.f, 0.f,	//top left
	//	1.f, 1.f,	//bottom right
	//	1.f, 1.f,	//bottom right
	//	0.f, 0.f,	//top left
	//	1.f,0.f		//top right
	//}; 
	float size = 0.3f;

	GSOutput output;
	for (int i = 0; i < 4; i++)
	{
		float4 pos = input[0].PosCS;
		pos.xy += size * (offsets[i] - float2(size, size));
		output.PosCS = mul(pos,proj); //
		output.Tex= offsets[i];
		output.Color= input[0].Color;

		//emitVertex();
		theOutput.Append(output);
	}
	//theOutput.RestartStrip();
}

