struct VS_OUT
{
	float4 screenPos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
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
cbuffer world : register(b2)
{
	float4x4 world;
}
cbuffer window : register(b3)
{
	int height;
	int width;
}
cbuffer Lights : register(b1)
{
	int index;
	int nrOfLights;
	AnyLight lights[16];
}
float2 poissonDisk[4] = {
	float2(-0.94201624, -0.39906216),
	float2(0.94558609, -0.76890725),
	float2(-0.094184101, -0.92938870),
	float2(0.34495938, 0.29387760)
};

float4 CalcLight(AnyLight light, float3 normal, float3 wPos, float3 LightDirection, float3 color, float visibility)
{
	//float3 LightDirection = light.direction.xyz;
	float ambientAmount = 0.2f;
	float4 ambientColor = float4(light.color.xyz, 1.0f)*ambientAmount;
	float diffuseFactor = max(0, dot(normal, -LightDirection));
	/*float theShade = diffuseFactor;
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
	diffuseFactor = max(theShade,0);*/ 
	float4 diffuseColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specularColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (diffuseFactor > 0)
	{
		
		float diffuseIntensity = light.color.w;
		diffuseColor = float4(light.color.xyz*diffuseIntensity*diffuseFactor, 1.0f);
		float3 vecToEye = normalize((camPos.xyz - wPos));
		float3 lightReflect = normalize(reflect(LightDirection, normal));
		float specularFactor = dot(vecToEye, lightReflect);
		if (specularFactor > 0)
		{
			specularFactor = pow(specularFactor, 32);
			float specularStrength = 0.5f;//in material
			specularColor = float4(light.color.xyz*specularStrength*specularFactor, 1.0f);
		}
	}

	return (ambientColor + ((1-visibility)*(diffuseColor + specularColor)));
}

float4 dirLight(float3 normal, AnyLight light, float3 wPos, float3 color, float visibility)
{
	return CalcLight(light, normal, wPos, light.direction.xyz, color, visibility);
}

float4 pointLight(int index, float3 normal, float3 wPos,float3 colour)
{
	float4 lightPos = mul(lights[index].worldLight, float4(0.0f, 0.0f, 0.0f, 1.0f));
	//lights[index].position = 
	float3 lightDir = wPos - lightPos.xyz;
	float distance = length(lightDir);

	lightDir = normalize(lightDir);

	float4 color = CalcLight(lights[index], normal, wPos, lightDir, colour,0);

	float attenuation = max(0, 1.0f - (distance / lights[index].position.w));// from 3d project

	float Attenuation = 1.0 +
		0.2f * distance +
		1.8f * distance * distance;
	return color / (Attenuation / lights[index].position.w);// color/attenuation in tutorial
}

Texture2D NormalTex : register(t0);
Texture2D Tex : register(t1);
Texture2D PositionTexture : register(t2);
Texture2D GlowTexture : register(t4);
Texture2D ShadowMap : register(t3);
SamplerState SampSt :register(s0);
SamplerState ShadowSamp : register(s1);
//SamplerState ShadowSampler
//{
//	// sampler state
//	Filter = MIN_MAG_MIP_LINEAR;
//	AddressU = CLAMP;
//	AddressV = CLAMP;
//	BorderColor = {
//		1,1,1,1
//	};
//
//	// sampler comparison state
//	ComparisonFunc = ALWAYS;
//};
float4 PS_main(VS_OUT input) : SV_Target
{


	//input.TexCoord = float2(input.screenPos.x / screenSize.x,input.screenPos.y / screenSize.y);//screenposition divided by screen size(Send it in)
	//float2((input.screenPos.x * 0.5) + 0.5, (input.screenPos.y * 0.5) + 0.5);
	if(index!=0)
		input.TexCoord = float2(input.screenPos.x / (width),input.screenPos.y / (height));
	//bumpNormal = BumpNormalTex.Sample(SampSt, input.TexCoord).xyz *2.0f - 1.0f;// back to [-1...1] 
	//colors = Tex.Sample(SampSt, input.TexCoord).xyz;
	float4 colorT = Tex.Sample(SampSt, input.TexCoord).xyzw;
	float3 pos = PositionTexture.Sample(SampSt, input.TexCoord).xyz;
	float4 glow = GlowTexture.Sample(SampSt, input.TexCoord).xyzw;
	float3 normal = NormalTex.Sample(SampSt, input.TexCoord).xyz*2.0f - 1.0f;
	float4 totalLight;
	if (length(normal) > 0)
	{
		if (nrOfLights > 0)
		{
			if (index == 0)
			{
				float4x4 biasMatrix = {

				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
				};
				//float4 shadowCoord=mul(float4(shadowCoord.xyz, 1.0f), world);
				float4 shadowCoord = mul(float4(pos.xyz, 1.0f), view);
				shadowCoord = mul(float4(shadowCoord.xyz, 1.0f), proj);
				//biasMatrix = transpose(biasMatrix);
				//shadowCoord = mul(float4(shadowCoord.xyz, 1.0f), biasMatrix);
				shadowCoord.z = shadowCoord.z / shadowCoord.w;
				shadowCoord.xy = (0.5f*shadowCoord.xy) + 0.5f;
				shadowCoord.y = abs(shadowCoord.y - 1);



				
				//float2 texShadowCoord;
				//texShadowCoord.x = shadowCoord.x / shadowCoord.w / 2.0f + 0.5f;
				//texShadowCoord.y = -shadowCoord.y / shadowCoord.w / 2.0f + 0.5f;
				//float visibility = 1.0f;
				//if ((saturate(texShadowCoord.x) == texShadowCoord.x) && (saturate(texShadowCoord.y) == texShadowCoord.y))
				//{
				//	float depthValue = ShadowMap.Sample(SampSt, texShadowCoord).r;

				//	float lightDepth = shadowCoord.z / shadowCoord.w;
				//	lightDepth = lightDepth - 0.001f;

				//	//shadowCoord = (0.5f*shadowCoord) + 0.5f;
				//	/*shadowCoord -= 1;
				//	shadowCoord = abs(shadowCoord);*/
				//	
				//
				//	//visibility = 0;
				//	if (lightDepth < depthValue)
				//	{
				//		visibility = 0.5f;
				//	}
				//}


				
				float visibility = 0.0;
				//float cosTheta = dot((normal), (lights[0].direction.xyz));
				//float bias = 0.005*tan(acos(cosTheta));
				//max(0.05 * (1.0 - dot(normal, lightDir)), 0.015);
				//bias = clamp(bias, 0,0.05);
				float bias;
				bias = max(0.030 * (1.0 - dot(normal, lights[0].direction.xyz)), 0.005);
				/*for (int i = 0; i < 4; i++)
				{
					if (ShadowMap.Sample(SampSt, shadowCoord.xy + poissonDisk[i] / 700.0).x < shadowCoord.z - bias)
					{
						visibility -= 0.20f;
					}
				}*/
				//if (ShadowMap.Sample(SampSt, shadowCoord.xy /*+ (poissonDisk[i] / 700.0)*/).x < shadowCoord.z - bias)
				//{
				//	visibility = 0.5f;
				//}
				int width;
				int height;
				int nrOfLevels;
				ShadowMap.GetDimensions(0, width, height, nrOfLevels);
				float2 textureSize = float2(width, height);
				float2 texelSize = 1.0 / textureSize;
				for (int x = -1; x <= 1; ++x)
				{
					for (int y = -1; y <= 1; ++y)
					{
						float pcfDepth = ShadowMap.Sample(ShadowSamp, shadowCoord.xy + float2(x, y) * texelSize).r;
						visibility += shadowCoord.z - bias > pcfDepth ? 1.0f: 0.0;
					}
				}
				visibility /= 9.0;
				//visibility += ShadowMap.SampleCmpLevelZero(ShadowSampler, shadowCoord.xy, shadowCoord.z);
				if (shadowCoord.z > 1.0)
					visibility = 0.0f;
				/*if (visibility > 0.9f)
					visibility = 0.9f;*/
				totalLight = dirLight(normal, lights[0], pos, colorT.xyz, visibility);

			}
			else
			{
				totalLight = pointLight(index, normal, pos, colorT.xyz);
				//totalLight = float4(0, 0, 0, 0);
			}

			//float4 colorT = float4(Tex.Sample(SampSt, input.Tex).xyz *totalLight.xyz, Tex.Sample(SampSt, input.Tex).w);

			/*if (glow.x != 0 || glow.y != 0 || glow.z != 0)
			{
				return saturate(glow+colorT);
			}*/
			colorT = float4(colorT.xyz *totalLight.xyz, 1.0f);
			if (index == 0)
				colorT.xyz += glow.xyz;
		}
	}
	/*if (index > 0)
		return float4(1, 0, 0, 1);*/
	return float4(colorT.xyz,1.0f);
}