struct VS_OUT
{
	float4 Pos: SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 wPosition : POSITION;
	float4 NormalWS : NORMAL;
	float4 ShadowPos : SHADOWPOS;
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
	int index;
	int nrOfLights;
	AnyLight lights[16];
}

float4 CalcLight(AnyLight light,  float3 normal, float3 wPos, float3 LightDirection,float visibility)
{
	//float3 LightDirection = light.direction.xyz;
	float ambientAmount = 0.2f;
	float4 ambientColor = float4(light.color.xyz, 1.0f)*ambientAmount;
	float diffuseFactor = max(0,dot(normal, -LightDirection));
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
	float4 diffuseColor = float4(0.0f, 0.0f, 0.0f,0.0f);
	float4 specularColor = float4(0.0f, 0.0f, 0.0f,0.0f);

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
			float specularStrength = 0.0f;
			specularColor = float4(light.color.xyz*specularStrength*specularFactor, 1.0f);
		}
	}
	
	return (ambientColor + ((1 - visibility)*(diffuseColor + specularColor)));
}

float4 dirLight(float3 normal, AnyLight light, float3 wPos,float visibility)
{
	return CalcLight(light, normal, wPos, light.direction.xyz, visibility);
}

float4 pointLight(int indexL, float3 normal, float3 wPos)
{
	float4 lightPos= mul(lights[indexL].worldLight,float4(0.0f, 0.0f, 0.0f, 1.0f));
	//lights[index].position = 
	float3 lightDir = wPos - lightPos.xyz;
	float distance = length(lightDir);

	lightDir = normalize(lightDir);

	float4 color = CalcLight(lights[indexL], normal, wPos,lightDir,0);

	float attenuation = max(0, 1.0f - (distance / lights[indexL].position.w));// from 3d project

	float Attenuation = 1.0f +
		0.2f * distance +
		1.8f * distance * distance;

	return color / (Attenuation/lights[indexL].position.w);// color/attenuation in tutorial
}

Texture2D Tex:register(t0);
Texture2D ShadowMap:register(t4);
SamplerState SampSt :register(s0);
SamplerState ShadowSamp :register(s1);
float4 PS_main(VS_OUT input) : SV_Target
{

	float3 normal = input.NormalWS.xyz;
	float4 texColor = Tex.Sample(SampSt, input.Tex).xyzw;

	float4 shadowCoord = input.ShadowPos;
	shadowCoord.z = shadowCoord.z / shadowCoord.w;
	shadowCoord.xy = (0.5f*shadowCoord.xy) + 0.5f;
	shadowCoord.y = abs(shadowCoord.y - 1);
	float visibility = 0.0;
	float bias;
	bias = max(0.030 * (1.0 - dot(normal, lights[0].direction.xyz)), 0.005);
	
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
			visibility += shadowCoord.z - bias > pcfDepth ? 1.0f : 0.0;
		}
	}
	visibility /= 14.0;
	if (shadowCoord.z > 1.0)
		visibility = 0.0f;

	float4 totalLight = dirLight(normal, lights[0],input.wPosition.xyz, visibility);

	for (int i = 1; i < nrOfLights; i++)
	{
		totalLight += pointLight(i, normal, input.wPosition.xyz);
	}

	float4 colorT = float4(texColor.xyz *totalLight.xyz, texColor.w);

	return colorT;
}