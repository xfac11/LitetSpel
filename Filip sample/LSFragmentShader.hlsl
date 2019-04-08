cbuffer LightProps : register(b0)
{
	float4x4 worldLight;
	float4 position; // w/a = Radius/Range
	float4 color; // w/a = intensity
	float4 cameraPos;// for specular calc
	float4 direction;// directional light w value holds the type of light 
	//POINTLIGHT = 0, DIRECTIONALLIGHT = 1, SPOTLIGHT = 2
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

//void GetGBufferAttributes(in float2 texCoord, out float3 normal,
//	out float3 position, out float3 diffuseAlbedo, out float3 specularAlbedo,
//	out float specularPower)
//{
//
//	//int3 sampleIndices = int3(screenPos.xy, 0);
//	//float3 color = Tex.Sample(SampSt, input.Tex).xyz;
//	//normal = NormalTex.Load(sampleIndices).xyz;
//	//position = PositionTexture.Load(sampleIndices).xyz;
//	//diffuseAlbedo = Tex.Load(sampleIndices).xyz;
//
//	normal = NormalTex.Sample(SampSt, texCoord).xyz;
//	position = PositionTexture.Sample(SampSt,texCoord).xyz;
//	diffuseAlbedo = Tex.Sample(SampSt, texCoord).xyz;
//
//	float4 spec = float4(1.0f, 1.0f, 1.0f, 1.0f);
//	specularAlbedo = spec.xyz;
//	specularPower = spec.w;
//}

//float3 CalcLighting(in float3 normal, in float3 position, in float3 diffuseAlbedo,
//	in float3 specularAlbedo, in float3 specularPower)
//{
//	float3 L = 0;
//	float attenuation = 1.0f;
//	//L = light - position;
//
//	float dist = length(L);
//	//attenuation = max(0, 1.0f - (dist / radius));
//
//	L /= dist;
//
//	float nDotL = saturate(dot(normal, L));
//	float3 diffuse = nDotL * color *diffuseAlbedo;
//
//	
//	//Specular here
//
//	/*
//	if ((diffuse*attenuation).z == 0.0f)
//	{
//		return float3(1.0f, 1.0f, 1.0f);
//	}*/
//	
//	return(diffuse);//*attenuation);
//}

//struct LightResult
//{
//	float4 Diffuse;
//	float4 Specular;
//};
//float4 diffuse(float4 lightC, float3 lightDir, float3 normal)
//{
//	float NdotL = max(0, dot(normal, lightDir));
//	return lightC * NdotL*lightPos.w;
//}
//float4 specular(float4 lightC, float3 viewDir, float3 lightDir, float3 normal, float specularPow)
//{
//	float3 reflection = normalize(reflect(-lightDir, normal));
//	float RdotV = max(0, dot(reflection, viewDir));
//
//	return lightC*pow(RdotV, specularPow);
//}
//float attenuation(float3 lightDir)
//{
//	return 1.0f / length(lightDir);
//}
//LightResult pointLight(float4 lightC, float4 lightPos, float3 viewDir, float4 position, float3 normal,float specPow)
//{
//	LightResult result;
//
//	float3 L = (lightPos.xyz - position.xyz).xyz;
//	float distance = length(L);
//
//	float atten = attenuation(L);
//	L = L / distance;
//
//	result.Diffuse = diffuse(lightC, L, normal) * atten;
//	result.Specular = specular(lightC, viewDir, L, normal,specPow) * atten;
//
//	return result;
//}
//LightResult computeLight(float4 position, float3 normal,float specPow)
//{
//	float3 V = normalize(float4(camPos,1.0f) - position).xyz;
//
//	LightResult totalResult = { {0, 0, 0, 0}, {0, 0, 0, 0} };
//
//	float4 lightPW = mul(float4(lightPos.xyz, 1.0f), world);
//	LightResult result = { {0, 0, 0, 0}, {0, 0, 0, 0} };
//	result = pointLight(color ,lightPW, V, position, normal,specPow);
//	totalResult.Diffuse += result.Diffuse;
//	totalResult.Specular += result.Specular;
//
//	totalResult.Diffuse = saturate(totalResult.Diffuse);
//	totalResult.Specular = saturate(totalResult.Specular);
//
//	return totalResult;
//}

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

	//In the light pixel shader we start by retrieving the color data and normals for this pixel using the point sampler.
	//float2 test = float2(0.0f, 1.0f);
	// Sample the colors from the color render texture using the point sampler at this texture coordinate location.
	input.TexCoord = input.screenPos.xy/768.0f;//screenposition divided by screen size(Send it in)
	//float2((input.screenPos.x * 0.5) + 0.5, (input.screenPos.y * 0.5) + 0.5);

	bumpNormal = BumpNormalTex.Sample(SampSt, input.TexCoord).xyz *2.0f - 1.0f;// back to [-1...1] 
	colors = Tex.Sample(SampSt, input.TexCoord).xyz;
	posCol = PositionTexture.Sample(SampSt, input.TexCoord).xyz;
	if (choice == 1)
	{
		return NormalTex.Sample(SampSt, input.TexCoord);
	}
	else if (choice == 2)
	{
		return float4(colors,1.0f);
	}
	else if (choice == 3)
	{
		return float4(posCol,1.0f);
	}	 
	else if (choice == 4)
	{
		return BumpNormalTex.Sample(SampSt, input.TexCoord);//*2.0f - 1.0f;
	}
	// Sample the normals from the normal render texture using the point sampler at this texture coordinate location.
	normals = NormalTex.Sample(SampSt, input.TexCoord).xyz *2.0f - 1.0f;// back to [-1...1] 
	if (length(normals) > 0.0f) //normals with that 
	{
		
		float3 final_colour = float3(0.2f, 0.2f, 0.2f);
		float3 ambient = colors * final_colour;
		float3 lightPW = mul(float4(0.0f,0.0f,0.0f, 1.0f), world).xyz;


		float3 vecToLight = lightPW - posCol;
		float d = length(vecToLight); //distance
		if (d <= position.w)
		{


			vecToLight /= d;
			float howMuchLight = dot(bumpNormal, vecToLight); //bumpNormal


			float specularStrength = 0.5f;
			float3 viewDir = normalize(camPos.xyz - posCol);
			float3 reflectDir = reflect(-vecToLight, bumpNormal);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			float3 specular = specularStrength * spec * color .xyz;

			lightIntensity = saturate(dot(bumpNormal, -vecToLight));
			float diffuse = max(howMuchLight, 0); //smooth
			float3 diffusefinal = saturate(colors * color .xyz*diffuse * position.w *(1 / d));
			final_colour = float3(ambient + diffusefinal + specular);
			final_colour = saturate((final_colour));
			//}

			// UPDATE THIS LINE TO ACCOUNT FOR SATURATION (PIXEL COLOUR CANNOT GO OVER 1.0)
			final_colour = min(final_colour, float3(1.0, 1.0, 1.0));
			return float4(final_colour, 1.0f);
		}
		else
		{
			return float4(0.0f,0.0f,0.0f, 1.0f);
		}
	}
	


	return float4(colors, 1.0f);
	/*//float4 output;
	////output = float4(light,1.0f);
	//output = float4(0.0f,0.0f,0.0f, 1.0f);
	////output = float4(normals, 1.0f);
	//


	//float4 bumpNormalS = BumpNormalTex.Sample(SampSt, input.TexCoord) *2.0f - 1.0f; //to -1...1
	//float4 surPos = PositionTexture.Sample(SampSt, input.TexCoord);
	//float4 surColor = Tex.Sample(SampSt, input.TexCoord);
	//float4 normal = NormalTex.Sample(SampSt, input.TexCoord)* 2.0f - 1.0f;  //to -1...1
	//if (length(normals) <= 0.0f)
	//{
	//	return surColor;
	//}

	//float specPow = 0.5f;
	//LightResult lit = computeLight(surPos, normalize(bumpNormalS.xyz),specPow);
	//
	//float4 ambient = float4(0.2f, 0.2f, 0.2f, 0.0f);
	//float4 diffuse = lit.Diffuse;
	//float4 specular = lit.Specular;
	//float4 emissive = float4(0.0f, 0.0f, 0.0f, 0.0f);//Object lit


	//float4 finalColor = (emissive + ambient + diffuse + specular) * surColor;

	//return finalColor;
	*/
}