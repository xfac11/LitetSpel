#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable


layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in; //Threads

struct Particle {
	vec4 pos;
	vec4 direction;
//	vec4 color;
//	float lifetime;
	int isDead;
	int speed;
};
layout (std430, binding = 0) buffer particles {
	Particle particleData[];
};

uniform float dt;

void main(){
uint index = gl_GlobalInvocationID.x;

	if(particleData[index].pos.y < -5.0){ //Should probably be exchanged for a lifetime variable
		particleData[index].pos = particleData[index].pos - particleData[index].pos * particleData[index].direction; //Reset to original position
	}

	if(particleData[index].isDead == 0){ //If the particle is not marked as dead
		particleData[index].pos -= (particleData[index].direction * particleData[index].speed) * dt;
	}
}