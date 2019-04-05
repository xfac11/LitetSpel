struct GSInput
{
	float4 pos : SV_POSITION;
	//float3 WorldPos : WORLD_POSITION;
	//float3 color : COLOR;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float3 thePoint : MYVALUE;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	//float3 color : COLOR;
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
void GS_main(triangle GSInput input[3], inout TriangleStream<GSOutput> theOutput)
{
	GSOutput output;

	float3 u = input[1].pos.xyz - input[0].pos.xyz;
	float3 v = input[2].pos.xyz - input[0].pos.xyz; //pos of 3 == 0
	float3 normal = mul( float4(normalize(cross(u, v)),0.0f), worldMat).xyz;

	//for (int j = 0; j < 2; j++)
	//{

	float3 tempPos = mul(float4(input[0].pos.xyz,1.0f), worldMat).xyz;
	float3 direction = camPos -tempPos;
	if ((dot(normal,direction)) >= 0.f)
	{
		for (int i = 0; i < 3; i++)
		{
			//if (j == 0)
			//{
			output.pos = mul(WorldViewProj, input[i].pos);
			output.thePoint = mul((float3x3)WorldViewProj, input[i].pos.xyz);
			//}
				//if (j == 1)
				//{
				//	output.pos = mul(worldMat, input[i].pos + float4(normal, 0.f));
				//	output.thePoint = mul((float3x3)worldMat, input[i].pos.xyz + normal);
				//}
			//output.color = input[i].color;
			output.tex = input[i].tex;
			output.normal = normalize(mul((float3x3)WorldViewProj, normal));
			theOutput.Append(output);

		}
		//}
	}
	theOutput.RestartStrip();
}

