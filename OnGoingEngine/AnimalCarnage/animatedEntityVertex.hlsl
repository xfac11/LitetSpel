
struct VS_IN
{
	float3 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;

	int3 JointIndices : JOINTS;
	float3 Weights : WEIGHTS;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 Binormal : BINORMAL;
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

//buffer for jointsTransformations[MAX_JOINTS];
//buffer proj

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	const int MAX_JOINTS = 50;
	const int MAX_WEIGHTS = 3;
	VS_OUT output = (VS_OUT)0;

	float4 totalLocalPos = 0.f;
	float4 totalNormal = 0.f;

	for (int i = 0; i < MAX_WEIGHTS; i++)
	{
		//float4x4 jointTransform = jointBuffer[input.JointIndices[i]]; //make buffer
		//float4 posePosition = jointTransform * float4 (input.Pos, 1.f);
		//totalLocalPos += posePosition * input.weights[i];

		//float4 worldNormal = jointTransform * float4(input.Normal, 0.f);
		//totalNormal += worldNormal * input.Weights[i];
	}

	output.Pos = mul(proj, mul(view, totalLocalPos));

	//output.Pos = float4(input.Pos, 1.0);
	////output.WorldPos = mul((float3x3)worldMat, input.Pos.xyz);

	////output.Color = input.Color;
	//output.Tex = input.Tex;
	//output.Normal = float4(input.Normal, 1.0f);
	//output.Tangent = float4(input.Tangent, 1.0f);
	//output.Binormal = float4(input.Binormal, 1.0f);
	return output;
}