#include "ParticleSystem.h"
#include <windows.h>
#include "glew/include/GL/glew.h"
#include <gl/GL.h>
#include <cstdlib>
#include <ctime>

ParticleSystem::ParticleSystem() {
	this->particleCount = 0;
	this->textureID = 0;
	this->vaoID = 0;
	this->vboID = 0;
	this->vboPos = 0;
	this->vboVel = 0;
	this->timer = 0.0;
	this->emitRate = 0.1;

	initParticles(); //Gives rendom positions to particles

	addParticle();
	createBuffers();
}


ParticleSystem::~ParticleSystem() {
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);

	glDeleteBuffers(1, &vboPos);
	glDeleteBuffers(1, &vboVel);
}

void ParticleSystem::createBuffers() {
	glGenVertexArrays(1, &vaoID);

	glGenBuffers(1, &vboID);
	glGenBuffers(1, &vboPos);
	glGenBuffers(1, &vboVel);
}

void ParticleSystem::prepareBuffers() {
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_COPY); //Data that will be modified by compute shader

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), BUFFER_OFFSET(sizeof(float) * 4));
}

void ParticleSystem::prepare(unsigned int& shaderProg) {
	glBindVertexArray(vaoID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
}

void ParticleSystem::initParticles() {
	std::srand(static_cast <unsigned> (std::time(0)));

	static float min = -2.0;
	static float max = 4.0;

	for (int i = 0; i < MAX_PARTICLES; i++) {
		float r1 = min + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / max);
		float r2 = min + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / max);
		this->particles[i].startPos = glm::vec4(r1, 0.0, r2, 1.0);
		this->particles[i].pos = this->particles[i].startPos;
	}
}

void ParticleSystem::addParticle() {
	this->particleCount++;
	//OutputDebugStringA("Particle added\n");
}

void ParticleSystem::update(double dt) {
	this->timer -= 1.0 * dt;
	if (this->particleCount < MAX_PARTICLES && this->timer <= 0) {
		addParticle();
		this->timer = this->emitRate;
	}
}

unsigned int ParticleSystem::getCount() const{
	return this->particleCount;
}

std::string ParticleSystem::getTexPath() const {
	return this->texPath;
}