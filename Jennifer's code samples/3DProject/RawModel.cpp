#include "RawModel.h"
#include <windows.h>
#include "glew/include/GL/glew.h"
#include <gl/GL.h>
#include <iostream>

RawModel::RawModel(const char* filePath) {
	this->vaoID = 0;
	this->vboID = 0;
	this->vertCount = 0;
	this->path = filePath;

	this->texturePath = "Path not loaded";
	this->normalTexPath = "Path not loaded";
	this->blendmapPath = "Path not loaded";
	this->rTexPath = "Path not loaded";
	this->gTexPath = "Path not loaded";
	this->bTexPath = "Path not loaded";
	this->textureID = 0;
	this->blendTexID = 0;
	this->rTexID = 0;
	this->gTexID = 0;
	this->bTexID = 0;

	this->ambientVal = { 0.0f, 0.0f, 0.0f };
	this->diffuseVal = { 0.0f, 0.0f, 0.0f };
	this->specularVal = { 0.0f, 0.0f, 0.0f };
	this->ambID = 0;
	this->diffID = 0;
	this->specID = 0;
	this->materialPath = "Path not loaded";
	this->worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->worldRotation = 0.0f;

	createBuffers();
}

RawModel::~RawModel() {
	OutputDebugStringA("Destructor is run for RawModel\n");
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
}

int RawModel::getVertCount() const {
	return this->vertCount;
}

void RawModel::setVertCount(int count) {
	this->vertCount = count;
}

void RawModel::setTextureID(unsigned int id) {
	this->textureID = id;
}

unsigned int RawModel::getNormalID() const
{
	return this->normalID;
}

void RawModel::setNormalID(unsigned int id)
{
	this->normalID = id;
}

glm::vec3 RawModel::getWorldPosition() const {
	return this->worldPosition;
}

void RawModel::setWorldPosition(glm::vec3 position) {
	this->worldPosition = position;
}

float RawModel::getWorldRotation() const {
	return this->worldRotation;
}

void RawModel::setWorldRotation(float rotation) {
	this->worldRotation = rotation;
}

void RawModel::prepareMaterials() {
	this->ambID = glGetUniformLocation(this->shaderProg, "ambient_val"); //Assign ID
	this->diffID = glGetUniformLocation(this->shaderProg, "diffuse_val");
	this->specID = glGetUniformLocation(this->shaderProg, "specular_val");
}

void RawModel::prepare(unsigned int& shaderProgram) {
	this->shaderProg = shaderProgram;
	prepareMaterials();

	glBindVertexArray(vaoID); // bind is like "enabling" the object to use it

	glBindBuffer(GL_ARRAY_BUFFER, vboID); // Bind the buffer ID as an ARRAY_BUFFER

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void RawModel::createBuffers() {
	glGenVertexArrays(1, &vaoID); // Vertex Array Object (VAO), description of the inputs to the GPU 

	glGenBuffers(1, &vboID); // create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
}

void RawModel::prepareBuffers() {
	glBindVertexArray(vaoID); // bind is like "enabling" the object to use it
	glBindBuffer(GL_ARRAY_BUFFER, vboID); // Bind the buffer ID as an ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, getVertCount() * VERTEX_SIZE, vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(sizeof(float) * 5));
}

const char* RawModel::getPath() const {
	return this->path;
}

std::string RawModel::getTexturePath() const {
	return this->texturePath;
}

std::string RawModel::getNormalTexturePath() const
{
	return this->normalTexPath;
}

void RawModel::setTexturePath(std::string texPath) {
	this->texturePath = texPath;
}

void RawModel::setNormalTexturePath(std::string texPath){
	this->normalTexPath = texPath;
}

void RawModel::setMaterialPath(std::string matPath) {
	this->materialPath = matPath;
}

std::string RawModel::getMaterialPath() const {
	return this->materialPath;
}

unsigned int RawModel::getTextureID() const {
	return this->textureID;
}
