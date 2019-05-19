
//The vertical blur shader requires the screen(or render to texture) height instead of width that the horizontal blur shader used.Note the buffer is called the same as in the horizontal shader but the height is used in the first variable instead of the width.

cbuffer screen : register(b0)
{
	int height;
	int width;
};

struct VS_IN
{
	float4 screenPos : POSITION;
	float2 TexCoord : TEXCOORD0;
};

struct VS_OUT
{
	float4 screenPos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
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

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;
	float texelSize;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.screenPos.w = 1.0f;

	output.TexCoord = input.TexCoord;
	output.screenPos = float4(output.TexCoord.x * 2 - 1, (1 - output.TexCoord.y) * 2 - 1, 0, 1);

	// Store the texture coordinates for the pixel shader.
	//output.TexCoord = input.TexCoord;

	//The texel size is based on the height value.

		// Determine the floating point size of a texel for a screen with this specific height.
		texelSize = 1.0f / height;

	//The offsets are modified by only the height value, the width stays at its current value.

		// Create UV coordinates for the pixel and its four vertical neighbors on either side.
	output.texCoord1 = input.TexCoord + float2(0.0f, texelSize * -4.0f);
	output.texCoord2 = input.TexCoord + float2(0.0f, texelSize * -3.0f);
	output.texCoord3 = input.TexCoord + float2(0.0f, texelSize * -2.0f);
	output.texCoord4 = input.TexCoord + float2(0.0f, texelSize * -1.0f);
	output.texCoord5 = input.TexCoord + float2(0.0f, texelSize *  0.0f);
	output.texCoord6 = input.TexCoord + float2(0.0f, texelSize *  1.0f);
	output.texCoord7 = input.TexCoord + float2(0.0f, texelSize *  2.0f);
	output.texCoord8 = input.TexCoord + float2(0.0f, texelSize *  3.0f);
	output.texCoord9 = input.TexCoord + float2(0.0f, texelSize *  4.0f);

	return output;
}
