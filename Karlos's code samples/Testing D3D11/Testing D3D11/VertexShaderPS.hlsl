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
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Color : COLOR;
};
struct VS_OUT
{
	//optimal layot should be
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Color : COLOR;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	//output.NormalWS = mul(float4(input.Normal, 0.0f), world);
	//output.NormalWS = normalize(output.NormalWS);
	//output.PosCS = mul(proj, float4(input.Pos, 1.0f));
	//output.PositionWS = mul(float4(input.Pos, 1.0f), world);


	//output.PositionWS = mul(mul(float4(input.Pos, 1.0f), world), view);
	//output.PositionWS = mul(float4(input.Pos, 1.0f), world);
	output.Tex = input.Tex;
	if (input.PosCS.y < -1.0)
	{
		output.Color  = float3(1.0, 0.0, 1.0);
	}
	else
	{
		output.Color = float3(0.0, 0.0, 1.0);
	}
	input.PosCS.w = 1.f;
	//billboard
	output.PosCS = mul(view,input.PosCS );
	return output;
}