struct VS_OUT
{
	float4 Pos : SV_POSITION;
	//float3 Color : COLOR;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float3 ThePoint : MYVALUE;
};
Texture2D Tex:register(t0);
SamplerState SampSt :register(s0);
float4 PS_main(VS_OUT input) : SV_Target
{
	float3 color = Tex.Sample(SampSt, input.Tex).xyz;
	float3 final_colour = float3(0.2f, 0.2f, 0.2f);
	// diffuse, no attenuation.

	float3 ambient = color * final_colour;
	float3 light_pos = { 0.f,0.f,-3.f };
	float3 light_colour = { 1.f,1.f,1.f };
	// IMPLEMENT HERE DIFFUSE SHADING
	float3 normal = normalize(input.Normal);
	float3 vecToLight = normalize(light_pos - input.ThePoint);

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
	float diffuse = max(dot(normal, vecToLight),0); //smooth
	float3 diffusefinal = color * light_colour*diffuse * 1.f *(1 / length(vecToLight));
	final_colour = ambient + diffusefinal;

	// UPDATE THIS LINE TO ACCOUNT FOR SATURATION (PIXEL COLOUR CANNOT GO OVER 1.0)
		final_colour = min(final_colour,float3(1.0,1.0,1.0));

		return float4(final_colour,1.0f);
		//return float4(diffuse, diffuse, diffuse, diffuse);
}