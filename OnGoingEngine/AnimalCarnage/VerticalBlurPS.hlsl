struct PS_IN
{
	float4 screenPos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};
Texture2D GlowMap : register(t0);
SamplerState SampSt :register(s0);
float4 PS_main(PS_IN input) : SV_TARGET
{
	float weight0, weight1, weight2, weight3, weight4;
	float normalization;
	float4 color;

	//We use the same weighting system / values that the horizontal blur shader also used.

	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Initialize the color to black.
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine vertical pixels to the color by the specific weight of each.
	color += GlowMap.Sample(SampleType, input.texCoord1) * weight4;
	color += GlowMap.Sample(SampleType, input.texCoord2) * weight3;
	color += GlowMap.Sample(SampleType, input.texCoord3) * weight2;
	color += GlowMap.Sample(SampleType, input.texCoord4) * weight1;
	color += GlowMap.Sample(SampleType, input.texCoord5) * weight0;
	color += GlowMap.Sample(SampleType, input.texCoord6) * weight1;
	color += GlowMap.Sample(SampleType, input.texCoord7) * weight2;
	color += GlowMap.Sample(SampleType, input.texCoord8) * weight3;
	color += GlowMap.Sample(SampleType, input.texCoord9) * weight4;

	// Set the alpha channel to one.
	color.a = 1.0f;

	return color;
}
