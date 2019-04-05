struct GSInput
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 NormalWS : NORMALWS;
	float3 PositionWS : POSITIONWS;
	float3 TangentWS : TANGENTWS; //Normal maps
	float3 BinormalWS :BINORMALWS; //Normal maps
};

struct GSOutput
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 NormalWS : NORMALWS;
	float3 PositionWS : POSITIONWS;
	float3 TangentWS : TANGENTWS; //Normal maps
	float3 BinormalWS : BINORMALWS; //Normal maps
};

cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 world;//world
	float4x4 view;//view
	float4x4 proj;//proj
	float3 camPos;
	float padding;
};
[maxvertexcount(3)]
void GSmain(triangle GSInput input[3], inout TriangleStream<GSOutput> theOutput)
{
	GSOutput output;
	float3 pos0 = input[0].PositionWS;
	float3 pos1 = input[1].PositionWS; 
	float3 pos2 = input[2].PositionWS; 
	float3 u = pos1 - pos0;
	float3 v = pos2 - pos0;; //pos of 3 == 0
	float3 normal = cross(u, v);
	normal = normalize(normal);



	//backgface culling here
	float3 tempPos = mul(float4(pos0, 1.0f), world).xyz;
	float3 direction = camPos - input[0].PositionWS.xyz;
	if ((dot(normal, direction)) >= 0.f)
	{
		for (int i = 0; i < 3; i++)
		{
			output.PosCS = input[i].PosCS;
			//output.PosCS = mul(input[i].PosCS, proj);
			//output.PositionWS = mul(float4(input[i].PositionWS.xyz, 1.0f), world).xyz;
			output.PositionWS = input[i].PositionWS;
			//
			//output.NormalWS = input[i].NormalWS;
			output.NormalWS = normal;
			output.Tex = input[i].Tex;

			output.TangentWS = input[i].TangentWS;//normalize(input[i].Tangent, (float3x3)world));
			output.BinormalWS = input[i].BinormalWS;// normalize(mul(input[i].Binormal, (float3x3)world));

			theOutput.Append(output);
		}
	}
	//theOutput.RestartStrip();
}