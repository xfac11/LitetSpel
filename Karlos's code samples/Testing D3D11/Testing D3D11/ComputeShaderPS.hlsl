
struct ParticleType
{
	float3 position;
	float3 direction;
	float velocity;
	bool active;
	//add padding?
};

cbuffer Compute :register(b0)
{
	float deltaTime;
};


//RW is read/write
RWStructuredBuffer<ParticleType> particleData : register(u0);

[numthreads(1, 1, 1)]
void CS_main( uint3 DTid : SV_DispatchThreadID )
{
	uint index=particleData.IncrementCounter(); //????
	if (particleData[index].position.y < -10.0) { //Should probably be exchanged for a lifetime variable
		particleData[index].position = particleData[index].position - particleData[index].position * particleData[index].direction; //Reset to original position
	}

	if (particleData[index].active == true) { //If the particle is not marked as dead
		particleData[index].position -= (particleData[index].direction* particleData[index].velocity) * deltaTime;
	}
	
}