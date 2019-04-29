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
	int index;
	int nrOfLights;
	AnyLight lights[16];
}
float4 CalcLight(AnyLight light,  float3 normal, float3 wPos, float3 LightDirection)
{
	//float3 LightDirection = light.direction.xyz;
	float ambientAmount = 0.1f;
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
		float diffuseIntensity = 0.4f;
		diffuseColor = float4(light.color.xyz*diffuseIntensity*diffuseFactor, 1.0f);
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
	float4 lightPos= mul(lights[index].worldLight,float4(0.0f, 0.0f, 0.0f, 1.0f));
	//lights[index].position = 
	float3 lightDir = wPos - lightPos.xyz;
	float distance = length(lightDir);

	lightDir = normalize(lightDir);

	float4 color = CalcLight(lights[index], normal, wPos,lightDir);

	float attenuation = max(0, 1.0f - (distance / lights[index].position.w));// from 3d project

	float Attenuation = 1.0f +
		0.2f * distance +
		1.8f * distance * distance;

	return color / (Attenuation/lights[index].position.w);// color/attenuation in tutorial
}

Texture2D Tex:register(t0);
SamplerState SampSt :register(s0);
float4 PS_main(VS_OUT input) : SV_Target
{

	float3 normal = input.NormalWS.xyz;
	float4 totalLight = dirLight(normal, lights[0],input.wPosition.xyz);

	for (int i = 1; i < nrOfLights; i++)
	{
		totalLight += pointLight(i, normal, input.wPosition.xyz);
	}

	float4 colorT = float4(Tex.Sample(SampSt, input.Tex).xyz *totalLight.xyz, Tex.Sample(SampSt, input.Tex).w);

	return colorT;
}