struct PointLight
{
	float4x4 world;
	float4 lightPos;//.w=radius
	float4 lightColor;//.w=intensity
	//float radius;
	//float lightIntensity;
};
cbuffer LightProps : register(b0)
{
	float4x4 worldLight;
	float4 lightPos;//.w=radius
	float4 lightColor;//.w=intensity
	float4 LcamPos;
}

cbuffer CB_PER_FRAME : register(b1)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
	float3 camPos;
	float choice;
};
Texture2D NormalTex : register(t0);
Texture2D Tex : register(t1);
Texture2D PositionTexture : register(t2);
Texture2D BumpNormalTex : register(t3);
SamplerState SampSt :register(s0);
struct PS_IN
{
	float4 screenPos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

float4 PS_main(PS_IN input) : SV_Target0
{
	float3 colors;
	float3 bumpNormal;
	float3 normals;
	float lightIntensity;
	float3 posCol;
	int3 sampleIndices = int3(input.TexCoord.xy, 0);

	float4 nor = NormalTex.Sample(SampSt, sampleIndices.xy);
	float4 pos = PositionTexture.Sample(SampSt, sampleIndices.xy);
	float4 tex = Tex.Sample(SampSt, sampleIndices.xy);

	bumpNormal = BumpNormalTex.Sample(SampSt, input.TexCoord).xyz*2.0f - 1.0f;
	colors = Tex.Sample(SampSt, input.TexCoord).xyz;
	posCol = PositionTexture.Sample(SampSt, input.TexCoord).xyz;
	if (choice == 1)
	{
		return NormalTex.Sample(SampSt, input.TexCoord);
	}
	else if (choice == 2)
	{
		return float4(colors, 1.0f);
	}
	else if (choice == 3)
	{
		return float4(posCol, 1.0f);
	}
	else if (choice == 4)
	{
		return BumpNormalTex.Sample(SampSt, input.TexCoord);
	}
	normals = NormalTex.Sample(SampSt, input.TexCoord).xyz *2.0f - 1.0f;// back to [-1...1] 
	if (length(normals) > 0.0f) //normals with that 
	{
		float3 final_colour = float3(0.2f, 0.2f, 0.2f);
		float3 ambient = colors * final_colour;
		//for (int i = 0; i < 2; i++)
		//{


			float3 lightPW = mul(float4(lightPos.xyz, 1.0f), world).xyz;


			float3 vecToLight = lightPW - posCol;
			float d = length(vecToLight); //distance
			vecToLight /= d;
			float howMuchLight = dot(bumpNormal,vecToLight ); //bumpNormal , normals
				//cell-shading
				/*float theShade = max(dot(normal, normalize(vecToLight)),0);
					if( theShade < 0.2f)
						theShade=0.f;
					else if( theShade >= 0.2f && theShade < 0.4f)
						theShade=0.2f;
					else if( theShade >= 0.4f && theShade < 0.6f)
						theShade=0.4f;
					else if( theShade >= 0.6f && theShade < 0.8f)
						theShade=0.6f;
					else if( theShade >= 0.8f)
						theShade=0.8f;
					float diffuse = max(theShade,0);*/
			float specularStrength = 0.5f;
			float3 viewDir = normalize(camPos.xyz - posCol);
			float3 reflectDir = reflect(-vecToLight, bumpNormal); //normals

			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			float3 specular = specularStrength * spec * lightColor.xyz; //arr
			//lightIntensity = saturate(dot(bumpNormal, -vecToLight));
			float diffuse = max(howMuchLight, 0); //smooth
			float3 diffusefinal = saturate(colors * lightColor.xyz *diffuse * lightPos.w *(1 / d)); //array of for multiple light
			final_colour = float3(ambient + diffusefinal + specular);
			final_colour = saturate(final_colour);


			// UPDATE THIS LINE TO ACCOUNT FOR SATURATION (PIXEL COLOUR CANNOT GO OVER 1.0)
		final_colour = min(final_colour, float3(1.f, 1.f, 1.f));
		return float4(final_colour, 1.0f); //final_colour, normals, bumpNormal posCol, colors
	}


	return float4(colors, 1.0f);
}