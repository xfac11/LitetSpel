struct INPUT
{
	float3 position : POSITION;
};

struct OUTPUT
{
	float3 position : POSITION;
};

OUTPUT VS_main(INPUT input)
{
	OUTPUT output;
	output.position = input.position;
	return output;
}