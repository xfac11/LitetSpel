#include "Skybox.h"
#include <Windows.h>
#include "glew/include/GL/glew.h"
#include <gl/GL.h>

Skybox::Skybox(std::string folderPath) {
	this->vaoID = 0;
	this->vboID = 0;
	this->textureID = 0;
	this->folderPath = folderPath;

	this->faces[0] = folderPath + "/right.jpg";
	this->faces[1] = folderPath + "/left.jpg";
	this->faces[2] = folderPath + "/top.jpg";
	this->faces[3] = folderPath + "/bottom.jpg";
	this->faces[4] = folderPath + "/front.jpg";
	this->faces[5] = folderPath + "/back.jpg";

	createBuffers();
}


Skybox::~Skybox(){
	OutputDebugStringA("Destructor is run for Skybox\n");
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
}

void Skybox::prepare(unsigned int& shaderProgram) {
	glBindVertexArray(vaoID);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
}

void Skybox::createBuffers(){
	glGenVertexArrays(1, &vaoID);

	glGenBuffers(1, &vboID);
}

void Skybox::prepareBuffers() {
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(geom), geom, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), BUFFER_OFFSET(0));
}
