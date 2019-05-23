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
	bool hasSkeleton;
	float4x4 jointTransformations[80];
};

struct VS_IN
{
	float3 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	int4 Joint : JOINTS;
	float4 Weights : WEIGHTS;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 Binormal : BINORMAL;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	float4 totalLocalPos = float4(input.Pos, 1.0);
	float4 totalNormal = float4(input.Normal, 0.0);

	if (hasSkeleton == true)
	{
		output.Pos  = mul(jointTransformations[input.Joint.x], totalLocalPos) * input.Weights.x;
		output.Pos += mul(jointTransformations[input.Joint.y], totalLocalPos) * input.Weights.y;
		output.Pos += mul(jointTransformations[input.Joint.z], totalLocalPos) * input.Weights.z;
		output.Pos += mul(jointTransformations[input.Joint.w], totalLocalPos) * input.Weights.w;

		output.Normal  = mul(jointTransformations[input.Joint.x], totalNormal) * input.Weights.x;
		output.Normal += mul(jointTransformations[input.Joint.y], totalNormal) * input.Weights.y;
		output.Normal += mul(jointTransformations[input.Joint.z], totalNormal) * input.Weights.z;
		output.Normal += mul(jointTransformations[input.Joint.w], totalNormal) * input.Weights.w;

		//output.Normal = float4(input.Normal, 1.0f);
		//output.Pos = mul(proj, mul(view, mul(world, output.Pos)));
		//output.Normal = mul(mul(view, world), output.Normal);
	}
	else if(hasSkeleton==false)
	{
		output.Pos = float4(input.Pos, 1.0f);
		output.Normal = float4(input.Normal, 1.0f);
	}
	//output.WPos = mul(view, mul(world, output.Pos));

	output.Tex = input.Tex;
	output.Tangent = float4(input.Tangent, 1.0f);
	output.Binormal = float4(input.Binormal, 1.0f);

	return output;
}