struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 wPosition : POSITION;
	float4 NormalWS : NORMAL;
	float4 TangentWS : TANGENT;
	float4 BinormalWS : BINORMAL;
};
struct AnyLight
{
	float4x4 worldLight;
	float4 position; // w/a = Radius/Range
	float4 color; // w/a = intensity
	float4 cameraPos;// for specular calc w/a = numberoflights
	float4 direction;// directional light
};
cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 view;//view
	float4x4 proj;//proj
	float4 camPos;
}
cbuffer Lights : register(b1)
{
	int nrOfLights;
	AnyLight lights[16];
}
float4 CalcLight(AnyLight light,  float3 normal, float3 wPos, float3 LightDirection)
{
	//float3 LightDirection = light.direction.xyz;
	float ambientAmount = 0.2f;
	float4 ambientColor = float4(light.color.xyz, 1.0f)*ambientAmount;
	float diffuseFactor = dot(normal, -LightDirection);

	float4 diffuseColor = float4(0.0f, 0.0f, 0.0f,0.0f);
	float4 specularColor = float4(0.0f, 0.0f, 0.0f,0.0f);

	if (diffuseFactor > 0)
	{
		diffuseColor = float4(light.color.xyz*0.5f*diffuseFactor, 1.0f);
		float3 vecToEye = normalize((camPos.xyz - wPos));
		float3 lightReflect = normalize(reflect(LightDirection, normal));
		float specularFactor = dot(vecToEye, lightReflect);
		if (specularFactor > 0)
		{
			specularFactor = pow(specularFactor, 32);
			float specularStrength = 0.5f;
			specularColor = float4(light.color.xyz*specularStrength*specularFactor, 1.0f);
		}
	}
	return (ambientColor + diffuseColor + specularColor);
}

float4 dirLight(float3 normal, AnyLight light, float3 wPos)
{
	return CalcLight(light, normal, wPos, light.direction.xyz);
}

float4 pointLight(int index, float3 normal, float3 wPos)
{
	float4 lightPos= mul(lights[index].worldLight, float4(0.0f, 0.0f, 0.0f, 1.0f));
	//lights[index].position = 
	float3 lightDir = wPos - lightPos;
	float distance = length(lightDir);

	lightDir = normalize(lightDir);

	float4 color = CalcLight(lights[index], normal, wPos,lightDir);

	float attenuation = max(0, 1.0f - (lightDir / lights[index].position.w));// from 3d project

	float Attenuation = 1.0f +
		0.2f * distance +
		1.8f * distance * distance;

	return color / (Attenuation/lights[index].position.w);// color/attenuation in tutorial
}

Texture2D Tex:register(t0);
SamplerState SampSt :register(s0);
float4 PS_main(VS_OUT input) : SV_Target
{

	float3 normal = normalize(input.NormalWS);
	float4 totalLight = dirLight(normal, lights[0],input.wPosition.xyz);

	for (int i = 1; i < nrOfLights; i++)
	{
		totalLight += pointLight(i, normal, input.wPosition.xyz);
	}

	float4 colorT = float4(Tex.Sample(SampSt, input.Tex).xyz*totalLight.xyz,Tex.Sample(SampSt,input.Tex).w);


	//float4 colorT = float4(0.0f,1.0f,0.0f,1.0f);
	//float3 final_colour = float3(0.2f, 0.2f, 0.2f);
	//// diffuse, no attenuation.

	////float3 ambient = color * final_colour;
	////float3 light_pos = { 0.f,2.0f,-10.f };
	////
	//////light_pos=mul((float3x3)world, light_pos);//sending the worldmat to the fragment shader 
	////float3 light_colour = { 1.f,1.f,1.f };
	////// IMPLEMENT HERE DIFFUSE SHADING
	//float3 normal = normalize(input.Normal);	
	////for (int i = 0; i < 0; i++)
	////{
	////	light_pos = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), lights[i].worldLight).xyz;

	////	float3 final_colour = float3(0.2f, 0.2f, 0.2f);
	////	float3 ambient = color * final_colour;
	////	float3 vecToLight = light_pos - input.wPosition;
	////	float d = length(vecToLight); //distance
	////	vecToLight = normalize(vecToLight);
	////	if (d <= lights[i].position.w)
	////	{

	////		//float3 lightPW = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), world).xyz;


	////		/*float3 vecToLight = lightPW - posCol;
	////		vecToLight /= d;*/
	////		float howMuchLight = dot(input.Normal, vecToLight); //bumpNormal


	////		float specularStrength = 0.5f;
	////		float3 viewDir = normalize(camPos.xyz - posCol);
	////		float3 reflectDir = reflect(-vecToLight, bumpNormal);
	////		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	////		float3 specular = specularStrength * spec * color.xyz;

	////		lightIntensity = saturate(dot(bumpNormal, -vecToLight));
	////		float diffuse = max(howMuchLight, 0); //smooth
	////		float3 diffusefinal = saturate(lights[i].color.xyz * color.xyz*diffuse *  lights[i].position.w *(1 / d));
	////		final_colour = float3(ambient + diffusefinal + specular);
	////		final_colour = saturate((final_colour));
	////		//}

	////		// UPDATE THIS LINE TO ACCOUNT FOR SATURATION (PIXEL COLOUR CANNOT GO OVER 1.0)
	////		final_colour = min(final_colour, float3(1.0, 1.0, 1.0));
	////		return float4(final_colour, 1.0f);
	////	}
	////	else
	////	{
	////		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	////	}



	////}
	////return float4(colors, 1.0f);
	////cell-shading
	///*float theShade = max(dot(normal, normalize(vecToLight)),0);
	//	if( theShade < 0.2f)
	//		theShade=0.f;
	//	else if( theShade >= 0.2f && theShade < 0.4f)
	//		theShade=0.2f;
	//	else if( theShade >= 0.4f && theShade < 0.6f)
	//		theShade=0.4f;
	//	else if( theShade >= 0.6f && theShade < 0.8f)
	//		theShade=0.6f;
	//	else if( theShade >= 0.8f)
	//		theShade=0.8f;
	//	float diffuse = max(theShade,0);*/
	//float diffuse = max(dot(normal, vecToLight),0); //smooth
	//float3 diffusefinal = color * light_colour*diffuse * 1.f *(1 / length(vecToLight));
	//final_colour = float3(ambient + diffusefinal);

	//// UPDATE THIS LINE TO ACCOUNT FOR SATURATION (PIXEL COLOUR CANNOT GO OVER 1.0)
	//final_colour += float3(0.0f, 0.0f, 0.0f);//Pass in a colorvalue with rgba so we can change color and transparency in the engine
	//	final_colour = min(final_colour,float3(1.0,1.0,1.0));

		return colorT;
		//return float4(diffuse, diffuse, diffuse, diffuse);
}