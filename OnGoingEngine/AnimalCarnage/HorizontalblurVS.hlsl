
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

	// Calculate the position of the vertex against the world, view, and projection matrices
	output.TexCoord = input.TexCoord;
	output.screenPos = float4(output.TexCoord.x * 2 - 1, (1 - output.TexCoord.y) * 2 - 1, 0, 1);
	// Store the texture coordinates for the pixel shader.
	output.TexCoord = input.TexCoord;

	//Here is where we determine the texel size which is just one divided by the screen width(or render to texture width).With this value we can now determine the UV coordinates of each horizontal neighbor pixel.

		// Determine the floating point size of a texel for a screen with this specific width.
		texelSize = 1.0f / width;

	//Here is where we generate the UV coordinates for the center pixel and four neighbors on either side.We take the current texture coordinates and add the horizontal offset to all nine coordinates.The horizontal offset is the texel size multiplied by the distance of the neighbor.For example the neighbor that is 3 pixels to the left is calculated by texelSize * -3.0f.Note the vertical coordinate in the offset is just zero so we don't move off the horizontal line we are sampling on.

		// Create UV coordinates for the pixel and its four horizontal neighbors on either side.
	output.texCoord1 = input.TexCoord + float2(texelSize * -4.0f, 0.0f);
	output.texCoord2 = input.TexCoord + float2(texelSize * -3.0f, 0.0f);
	output.texCoord3 = input.TexCoord + float2(texelSize * -2.0f, 0.0f);
	output.texCoord4 = input.TexCoord + float2(texelSize * -1.0f, 0.0f);
	output.texCoord5 = input.TexCoord + float2(texelSize *  0.0f, 0.0f);
	output.texCoord6 = input.TexCoord + float2(texelSize *  1.0f, 0.0f);
	output.texCoord7 = input.TexCoord + float2(texelSize *  2.0f, 0.0f);
	output.texCoord8 = input.TexCoord + float2(texelSize *  3.0f, 0.0f);
	output.texCoord9 = input.TexCoord + float2(texelSize *  4.0f, 0.0f);

	return output;
}
