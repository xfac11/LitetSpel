
struct VS_IN
{
	float3 Pos : SV_POSITION;
	float3 Normal : NORMAL;

	float4 Joint : JOINTS;
	float4 Weights : WEIGHTS;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float4 WPos : WORLD_POS;
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
cbuffer skinningData : register(b2) //MAX_JOINTS
{
	float4x4 jointTransformations[80];
};
//buffer for jointsTransformations[MAX_JOINTS];
//buffer proj


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	float4 totalLocalPos = float4(input.Pos, 1.0);
	float4 totalNormal = float4(input.Normal, 0.0);

	//for (int i = 0; i < MAX_WEIGHTS; i++)
	//{
	//	//float4x4 jointTransform = jointBuffer[input.JointIndices[i]]; //make buffer
	//	//float4 posePosition = jointTransform * float4 (input.Pos, 1.f);
	//	//totalLocalPos += posePosition * input.weights[i];

	//	//float4 worldNormal = jointTransform * float4(input.Normal, 0.f);
	//	//totalNormal += worldNormal * input.Weights[i];
	//}

	output.Pos		 = mul(jointTransformations[input.Joint.x], totalLocalPos) * input.Weights.x;
	output.Pos		+= mul(jointTransformations[input.Joint.y], totalLocalPos) * input.Weights.y;
	output.Pos		+= mul(jointTransformations[input.Joint.z], totalLocalPos) * input.Weights.z;
	output.Pos		+= mul(jointTransformations[input.Joint.w], totalLocalPos) * input.Weights.w;

	output.Normal	 = mul(jointTransformations[input.Joint.x], totalNormal) * input.Weights.x;
	output.Normal	+= mul(jointTransformations[input.Joint.y], totalNormal) * input.Weights.y;
	output.Normal	+= mul(jointTransformations[input.Joint.z], totalNormal) * input.Weights.z;
	output.Normal	+= mul(jointTransformations[input.Joint.w], totalNormal) * input.Weights.w;

	output.WPos		 = mul(view, mul(world, output.Pos));
	output.Pos		 = mul(proj, mul(view, mul(world, output.Pos)));
	output.Normal	 = mul(mul(view, world), output.Normal);

	return output;
}