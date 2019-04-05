cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
	float3 camPos;
	float padding;
};

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

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;
	////output.screenPos = mul(proj,float4(input.screenPos, 1.0f));
	//output.screenPos = float4(input.screenPos, 1.0f);//mul(world,float4(input.screenPos, 1.0f));
	//output.TexCoord = input.TexCoord;

	output.TexCoord = input.TexCoord;

	// Change the position vector to be 4 units for proper matrix calculations.
	//input.scree.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	//output.screenPos = mul(float4(input.screenPos,1.0f), world);
	//output.screenPos = mul(output.screenPos, view);
	//output.screenPos = mul(float4(input.screenPos,1.0f), proj);
	output.screenPos = float4(output.TexCoord.x * 2 - 1, (1 - output.TexCoord.y) * 2 - 1, 0, 1); //<---fixed the issue(need to understand)
	//output.screenPos = mul(proj,float4(input.screenPos, 1.0f));
	//output.screenPos = float4(input.screenPos, 1.0f);
	// Store the texture coordinates for the pixel shader.

	//return output;



	return output;
}