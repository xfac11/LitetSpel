
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

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;
	float texelSize;


	input.screenPos.w = 1.0f;

	
	output.TexCoord = input.TexCoord;
	output.screenPos = float4(output.TexCoord.x * 2 - 1, (1 - output.TexCoord.y) * 2 - 1, 0, 1);
	output.TexCoord = input.TexCoord;


	texelSize = 1.0f / width;

	//Here is where we generate the UV coordinates for the center pixel and four neighbors on either side.
	//We take the current texture coordinates and add the horizontal offset to all nine coordinates.
	//The horizontal offset is the texel size multiplied by the distance of the neighbor.
	//For example the neighbor that is 3 pixels to the left is calculated by texelSize * -3.0f.
	//Note the vertical coordinate in the offset is just zero so we don't move off the horizontal line we are sampling on.

		// Create UV coordinates for the pixel and its four horizontal neighbors on either side.
		output.texCoord1 = input.TexCoord + float2(texelSize * 15.0f, 0.0f);
		output.texCoord2 = input.TexCoord + float2(texelSize * 14.0f, 0.0f);
		output.texCoord3 = input.TexCoord + float2(texelSize * 13.0f, 0.0f);
		output.texCoord4 = input.TexCoord + float2(texelSize * 12.0f, 0.0f);
		output.texCoord5 = input.TexCoord + float2(texelSize * 11.0f, 0.0f);
		output.texCoord6 = input.TexCoord + float2(texelSize * 10.0f, 0.0f);
		output.texCoord7 = input.TexCoord + float2(texelSize * 9.0f, 0.0f);
		output.texCoord8 = input.TexCoord + float2(texelSize * 8.0f, 0.0f);
		output.texCoord9 = input.TexCoord + float2(texelSize * 7.0f, 0.0f);
		output.texCoord10 = input.TexCoord + float2(texelSize * 6.0f, 0.0f);
		output.texCoord11 = input.TexCoord + float2(texelSize * 5.0f, 0.0f);
		output.texCoord12 = input.TexCoord + float2(texelSize * 4.0f, 0.0f);
		output.texCoord13 = input.TexCoord + float2(texelSize * 3.0f, 0.0f);
		output.texCoord14 = input.TexCoord + float2(texelSize * 2.0f, 0.0f);
		output.texCoord15 = input.TexCoord + float2(texelSize * 1.0f, 0.0f);
		output.texCoord16 = input.TexCoord + float2(texelSize * 0.0f, 0.0f);
		output.texCoord17 = input.TexCoord + float2(texelSize * 1.0f, 0.0f);
		output.texCoord18 = input.TexCoord + float2(texelSize * 2.0f, 0.0f);
		output.texCoord19 = input.TexCoord + float2(texelSize * 3.0f, 0.0f);
		output.texCoord20 = input.TexCoord + float2(texelSize * 4.0f, 0.0f);
		output.texCoord21 = input.TexCoord + float2(texelSize * 5.0f, 0.0f);
		output.texCoord22 = input.TexCoord + float2(texelSize * 6.0f, 0.0f);
		output.texCoord23 = input.TexCoord + float2(texelSize * 7.0f, 0.0f);
		output.texCoord24 = input.TexCoord + float2(texelSize * 8.0f, 0.0f);
		output.texCoord25 = input.TexCoord + float2(texelSize * 9.0f, 0.0f);
		output.texCoord26 = input.TexCoord + float2(texelSize * 10.0f, 0.0f);
		output.texCoord27 = input.TexCoord + float2(texelSize * 11.0f, 0.0f);
		output.texCoord28 = input.TexCoord + float2(texelSize * 12.0f, 0.0f);
		output.texCoord29 = input.TexCoord + float2(texelSize * 13.0f, 0.0f);
		output.texCoord30 = input.TexCoord + float2(texelSize * 14.0f, 0.0f);
		output.texCoord31 = input.TexCoord + float2(texelSize * 15.0f, 0.0f);


	return output;
}
