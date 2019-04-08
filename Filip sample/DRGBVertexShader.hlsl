cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 world;//world
	float4x4 view;//view
	float4x4 proj;//proj
	float3 camPos;
	float padding;
};

struct VS_IN
{
	float3 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMALPOS;
	float3 ThePoint : THEPOINT;
	float3 TangentWS : TANGENT; //Normal maps
	float3 BinormalWS :BINORMAL; //Normal maps
};

struct VS_OUT
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 NormalWS : NORMALWS;
	float4 PositionWS : POSITIONWS;
	float3 TangentWS : TANGENT; //Normal maps
	float3 BinormalWS :BINORMAL; //Normal maps
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;


	output.NormalWS = normalize(mul(float4((input.Normal), 0.0f), world)).xyz;//=


	output.PosCS = mul(proj, float4(input.Pos, 1.0f));
	output.PositionWS = mul(float4(input.Pos, 1.0f), world);


	output.Tex = input.Tex;

	output.BinormalWS = mul(world, float4(input.BinormalWS, 0.0f));
	output.TangentWS = mul(world, float4(input.TangentWS, 0.0f));
	output.TangentWS = normalize(output.TangentWS);

	output.BinormalWS = cross(output.NormalWS, output.TangentWS);
	output.BinormalWS = normalize(output.BinormalWS);

	return output;
}