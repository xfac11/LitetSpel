struct PS_IN
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 NormalWS : NORMALWS; //Normal maps
	float3 PositionWS : POSITIONWS;
	float3 TangentWS : TANGENTWS; //Normal maps
	float3 BinormalWS : BINORMALWS; //Normal maps
};
struct PS_OUT
{
	float4 Normal : SV_Target0;
	float4 TexColor : SV_Target1;
	float4 Pos : SV_Target2;
	float4 BumpNor : SV_Target3;
};
Texture2D Tex : register(t0);
SamplerState SampSt :register(s0);


//
//uniform sampler2D particleTex;
//
//in vec2 texCoords;
//in vec3 colorFinal;
//out vec4 finalColor;

float4 PS_main(PS_IN input) : SV_Target
{

	float4 color = Tex.Sample(SampSt, input.Tex);
	//return = texture(particleTex, texCoords) * vec4(colorFinal, 1.0);
	return color;// * (input color 1.0)
}