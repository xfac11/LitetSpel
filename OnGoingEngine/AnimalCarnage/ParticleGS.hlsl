struct OUTPUT
{
	float3 position : POSITION;
};

struct GSOutput
{
	float4 Pos: SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 view;//view
	float4x4 proj;//proj
	float4 camPos;
}

cbuffer CAMERA : register(b1)
{
	float3 upp;
}
cbuffer ParticleConfig : register(b2)
{
	float size;
}
[maxvertexcount(4)]
void GS_main(point OUTPUT input[1], inout TriangleStream<GSOutput> theOutput)
{
	float3 planeNormal = input[0].position - camPos.xyz;
	//planeNormal.y = 0.0f;
	planeNormal = normalize(planeNormal);
	float3 right = normalize(cross(planeNormal, upp));

	float3 up = /*normalize(upp);*/normalize(cross(planeNormal, right));
	float3 vert[4];
	vert[0] = input[0].position - right* size - up* size; // Bottom left
	vert[1] = input[0].position + right* size - up* size; // Bottom right
	vert[2] = input[0].position - right* size + up* size; // Top left
	vert[3] = input[0].position + right* size + up* size; // Top right Dunno why *2 but it works

	float2 texCoord[4];
	texCoord[2] = float2(0, 1);//2
	texCoord[3] = float2(1, 1);//3
	texCoord[0] = float2(0, 0);//0
	texCoord[1] = float2(1, 0);//1

	GSOutput output;
	for (int i = 0; i < 4; i++)
	{
		output.Pos = mul(float4(vert[i], 1.0f), view);
		output.Pos = mul(output.Pos, proj);
		output.uv = texCoord[i];

		theOutput.Append(output);
	}
}

