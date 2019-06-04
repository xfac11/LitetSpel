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
	float2 texCoord10 : TEXCOORD10;
	float2 texCoord11 : TEXCOORD11;
	float2 texCoord12 : TEXCOORD12;
	float2 texCoord13 : TEXCOORD13;
	float2 texCoord14 : TEXCOORD14;
	float2 texCoord15 : TEXCOORD15;
	float2 texCoord16 : TEXCOORD16;
	float2 texCoord17 : TEXCOORD17;
	float2 texCoord18 : TEXCOORD18;
	float2 texCoord19 : TEXCOORD19;
	float2 texCoord20 : TEXCOORD20;
	float2 texCoord21 : TEXCOORD21;
	float2 texCoord22 : TEXCOORD22;
	float2 texCoord23 : TEXCOORD23;
	float2 texCoord24 : TEXCOORD24;
	float2 texCoord25 : TEXCOORD25;
	float2 texCoord26 : TEXCOORD26;
	float2 texCoord27 : TEXCOORD27;
	float2 texCoord28 : TEXCOORD28;
	float2 texCoord29 : TEXCOORD29;
	float2 texCoord30 : TEXCOORD30;
	float2 texCoord31 : TEXCOORD31;

};
Texture2D GlowMap : register(t0);
SamplerState SampSt :register(s0);
float4 PS_main(PS_IN input) : SV_TARGET
{
	float weight0, weight1, weight2, weight3, weight4, weight5, weight6,weight7,weight8, weight9, weight10, weight11, weight12, weight13, weight14 ,weight15;
	float normalization = 1;
	float4 color;

	//We use the same weighting system / values that the horizontal blur shader also used.

	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 1.0f;
	weight1 = 0.98f;
	weight2 = 0.12f;
	weight3 = 0.12f;
	weight4 = 0.12f;
	weight5 = 0.12f;
	weight6 = 0.12f;
	weight7 = 0.12f;
	weight8 = 0.12f;
	weight9 = 0.12f;
	weight10 = 0.11f;
	weight11 = 0.010f;
	weight12 = 0.08f;
	weight13 = 0.06f;
	weight14 = 0.04f;
	weight15 = 0.02f;



	// Create a normalized value to average the weights out a bit.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4 + weight5 + weight6 + weight7));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;
	weight5 = weight5 / normalization;
	weight6 = weight6 / normalization;
	weight7 = weight7 / normalization;
	weight8 = weight8 / normalization;
	weight9 = weight9 / normalization;
	weight10 = weight10 / normalization;
	weight11 = weight11 / normalization;
	weight12 = weight12 / normalization;
	weight13 = weight13 / normalization;
	weight14 = weight14 / normalization;
	weight15 = weight15 / normalization;

	// Initialize the color to black.
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine vertical pixels to the color by the specific weight of each.


	color += GlowMap.Sample(SampSt, input.texCoord1) * weight15;
	color += GlowMap.Sample(SampSt, input.texCoord2) * weight14;
	color += GlowMap.Sample(SampSt, input.texCoord3) * weight13;
	color += GlowMap.Sample(SampSt, input.texCoord4) * weight12;
	color += GlowMap.Sample(SampSt, input.texCoord5) * weight11;
	color += GlowMap.Sample(SampSt, input.texCoord6) * weight10;
	color += GlowMap.Sample(SampSt, input.texCoord7) * weight9;
	color += GlowMap.Sample(SampSt, input.texCoord8) * weight8;
	color += GlowMap.Sample(SampSt, input.texCoord9) * weight7;
	color += GlowMap.Sample(SampSt, input.texCoord10)* weight6;
	color += GlowMap.Sample(SampSt, input.texCoord11)* weight5;
	color += GlowMap.Sample(SampSt, input.texCoord12)* weight4;
	color += GlowMap.Sample(SampSt, input.texCoord13)* weight3;
	color += GlowMap.Sample(SampSt, input.texCoord14)* weight2;
	color += GlowMap.Sample(SampSt, input.texCoord15)* weight1;
	color += GlowMap.Sample(SampSt, input.texCoord16)* weight0;
	color += GlowMap.Sample(SampSt, input.texCoord17)* weight1;
	color += GlowMap.Sample(SampSt, input.texCoord18)* weight2;
	color += GlowMap.Sample(SampSt, input.texCoord19)* weight3;
	color += GlowMap.Sample(SampSt, input.texCoord20)* weight4;
	color += GlowMap.Sample(SampSt, input.texCoord21)* weight5;
	color += GlowMap.Sample(SampSt, input.texCoord22)* weight6;
	color += GlowMap.Sample(SampSt, input.texCoord23)* weight7;
	color += GlowMap.Sample(SampSt, input.texCoord24)* weight8;
	color += GlowMap.Sample(SampSt, input.texCoord25)* weight9;
	color += GlowMap.Sample(SampSt, input.texCoord26)* weight10;
	color += GlowMap.Sample(SampSt, input.texCoord27)* weight11;
	color += GlowMap.Sample(SampSt, input.texCoord28)* weight12;
	color += GlowMap.Sample(SampSt, input.texCoord29)* weight13;
	color += GlowMap.Sample(SampSt, input.texCoord30)* weight14;
	color += GlowMap.Sample(SampSt, input.texCoord31)* weight15;


	// Set the alpha channel to one.
	color.a = 1.0f;

	return color;
}
