struct VS_IN
{
	float3 screenPos : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct VS_OUT
{
	float4 screenPos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
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
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;


	//output.screenPos = mul(proj,float4(input.screenPos,1.0f));
	//if (index != 0)
	//{
	//	//point light sphere
	//	output.screenPos = mul(proj, float4(input.screenPos, 1.0f));
	//	output.TexCoord = input.TexCoord;//float2((output.screenPos.x * 0.5) + 0.5, (output.screenPos.y * 0.5) + 0.5);
	//}
	//else
	//{	
	//	//directional light quad
	//	output.TexCoord = input.TexCoord;
	//	output.screenPos = float4(output.TexCoord.x * 2 - 1, output.TexCoord.y * 2 - 1, 0, 1);//<---fixed the issue(need to understand)
	//}
	//if (index == 0)
	//{
	//	output.TexCoord = input.TexCoord;
	//	output.screenPos = float4(output.TexCoord.x * 2 - 1, (1 - output.TexCoord.y) * 2 - 1, 0, 1);//<---fixed the issue(need to understand)
	//}
	//else
	//{
	//	output.screenPos = mul(proj, float4(input.screenPos, 1.0f));
	//	output.TexCoord = input.TexCoord;
	//}
	//output.screenPos = float4(output.TexCoord.x * 2 - 1, (1 - output.TexCoord.y) * 2 - 1, 0, 1);
	if (camPos.w == 0)
	{
		output.TexCoord = input.TexCoord;
		output.screenPos = float4(output.TexCoord.x * 2 - 1, (1 - output.TexCoord.y) * 2 - 1, 0, 1);
	}
	else
	{
		output.screenPos = mul(float4(input.screenPos, 1.0f), world);
		output.screenPos = mul(float4(output.screenPos.xyz, 1.0f),view);
		output.screenPos = mul(float4(output.screenPos.xyz, 1.0f),proj);
		output.TexCoord = input.TexCoord;
	}
	return output;
}