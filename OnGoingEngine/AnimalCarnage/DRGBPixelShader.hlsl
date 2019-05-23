struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 wPosition : POSITION;
	float4 NormalWS : NORMAL;
	float4 TangentWS : TANGENT;
	float4 BinormalWS : BINORMAL;

};
struct PS_OUT
{
	//float3 Color : COLOR;
	//float4 DiffuseAlbedo : DIFFUSE;
	//float4 SpecularAlbedo : SPECULAR;
	float4 Normal : SV_Target0;
	float4 TexColor : SV_Target1;
	float4 Pos : SV_Target2;
	float4 GlowTex : SV_Target3;
	//float4 BumpNor : SV_Target3;
	//float3 Tangent : TANGENT; //Normal maps
	//float3 Binormal : BINORMAL; //Normal maps

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
cbuffer texRepeat : register(b2)
{
	float4 repeat;//world
	float4 colorMask;
}

Texture2D Tex : register(t0);
Texture2D Nor : register(t1);
Texture2D Glow : register(t2);
Texture2D Mask : register(t3);
SamplerState SampSt :register(s0);
PS_OUT PS_main(PS_IN input)
{
	//Fix properties in the shaders. Entrypoint and shader type
	PS_OUT output;


	float4 textureColor = Tex.Sample(SampSt, input.Tex*repeat).xyzw;
	if (Mask.Sample(SampSt, input.Tex*repeat).w > 0.0f) //If there is relevant information in the mask sample
	{
		textureColor = textureColor * float4(colorMask.xyz, 1); //Add the mask to the texture (Works like an overlay)
	}
	float3 bumpNormal;

	//float3 diffuseAlbedo = Tex.Sample(SampSt, input.Tex).rgb;
	//float3 normalWS = normalize(input.NormalWS);

	//output.Normal = float4(input.NormalWS, 1.0f);//Normal in worldspace
	//output.Pos = float4(input.PositionWS, 1.0f);
	////output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	////output.SpecularAlbedo = float4(SpecularAlbedo, SpecularPower);
	//output.TexColor = Tex.Sample(SampSt, input.Tex);//float4(input.ThePoint, 1.0f);//World space position

	//output.TexColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
	float4 glowMap= Glow.Sample(SampSt, input.Tex);
	//output.TexColor = Tex.SampleLevel(SampSt, input.Tex,10);
	float3 normalColor = Nor.Sample(SampSt, input.Tex).xyz;
	//output.TexColor = float4(textureColor.xyz,1);
	/*if (glowMap.x != 0 || glowMap.y != 0 || glowMap.z != 0)
	{
		output.GlowTex = glowMap;
			
	}
	else
		output.GlowTex = float4(0, 0, 0, 0);*/
	output.TexColor = float4(textureColor.xyz, 1.0f);
	/*if (glowMap.x != 0 || glowMap.y != 0 || glowMap.z != 0)
	{
		glowMap.w = 1;
	}
	else
	{
		glowMap.w = 0;
	}*/
	output.GlowTex = glowMap;
	normalColor = normalColor * 2.f - 1.f;

	bumpNormal = (normalColor.x* input.TangentWS) + (normalColor.y*input.BinormalWS) + (normalColor.z*input.NormalWS);
	bumpNormal = normalize(bumpNormal);

	//  [-1...1] to [0...1]
	float x = (0.5f*input.NormalWS.x) + 0.5f;  //bumpNormal //input.NormalWS.xyz
	float y = (0.5f*input.NormalWS.y) + 0.5f;
	float z = (0.5f*input.NormalWS.z) + 0.5f;
	//float xP = (0.5f*input.PositionWS.x) + 0.5f;
	//float yP = (0.5f*input.PositionWS.y) + 0.5f;
	//float zP = (0.5f*input.PositionWS.z) + 0.5f;
	float bux = (0.5f*bumpNormal.x) + 0.5f;  //bumpNormal //input.NormalWS.xyz
	float buy = (0.5f*bumpNormal.y) + 0.5f;
	float buz = (0.5f*bumpNormal.z) + 0.5f;
	float xP = input.wPosition.x;
	float yP = input.wPosition.y;
	float zP = input.wPosition.z;
	float3 norScale = float3(x, y, z);
	output.Normal = float4(norScale, 1.0f); //bumpNormal
	output.Pos = float4(xP, yP, zP, 1.0f);
	//output.BumpNor = float4(bux, buy, buz, 1.0f);


	//output.Pos = mul(world,float4(input.PositionWS, 1.0f));

	return output;
}