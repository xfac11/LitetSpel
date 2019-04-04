#include "AnimatedModel.h"
#include <windows.h>
#include "glew/include/GL/glew.h"
#include <gl/GL.h>

AnimatedModel::AnimatedModel(std::string filePath) {
	this->path = filePath;
	this->texturePath = "Path not loaded";

	this->vaoID = 0;
	this->vboID = 0;
	this->vboIDJoints = 0;
	this->iboID = 0;

	this->textureID = 0;
	this->ambID = 0;
	this->diffID = 0;
	this->specID = 0;

	this->vertCount = 0;
	this->jointCount = 0;
	this->numIndices = 0;

	this->worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->worldRotation = 0.0f;

	this->ambientVal = { 0.0f, 0.0f, 0.0f };
	this->diffuseVal = { 0.0f, 0.0f, 0.0f };
	this->specularVal = { 0.0f, 0.0f, 0.0f };

	this->scene = nullptr;

	createBuffers();
}

AnimatedModel::~AnimatedModel() {
	OutputDebugStringA("Destructor is run for AnimatedModel\n");
	this->importer.FreeScene();
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &iboID);
}

void AnimatedModel::prepareMaterials() {
	this->ambID = glGetUniformLocation(this->shaderProg, "ambient_val"); //Assign ID
	this->diffID = glGetUniformLocation(this->shaderProg, "diffuse_val");
	this->specID = glGetUniformLocation(this->shaderProg, "specular_val");
}

void AnimatedModel::prepareJoints() {
	for (int i = 0; i < MAX_JOINTS; i++) {
		std::string name = "jointTransforms[" + std::to_string(i) + "]"; //Name in shader
		jointLocations[i] = glGetUniformLocation(this->shaderProg, name.c_str()); //Get an ID for all the possible joints to be used in shader
	}
}

void AnimatedModel::createBuffers() {
	glGenVertexArrays(1, &vaoID); // Vertex Array Object (VAO), description of the inputs to the GPU

	glGenBuffers(1, &vboID); // create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)

	glGenBuffers(1, &iboID); //Create index buffer

	glGenBuffers(1, &vboIDJoints); // create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
}

void AnimatedModel::prepare(unsigned int& shaderProgram) {
	this->shaderProg = shaderProgram;
	prepareMaterials();
	prepareJoints();

	glBindVertexArray(vaoID); // bind is like "enabling" the object to use it

	glBindBuffer(GL_ARRAY_BUFFER, vboID); // Bind the buffer ID as an ARRAY_BUFFER

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDJoints); // Bind the buffer ID as an ARRAY_BUFFER
}

void AnimatedModel::prepareBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, vboID); // Bind the buffer ID as an ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, getVertCount() * ANIM_VERTEX_SIZE, vertices.data(), GL_STATIC_DRAW);

	glBindVertexArray(vaoID); // bind is like "enabling" the object to use it
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ANIM_VERTEX_SIZE, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, ANIM_VERTEX_SIZE, BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, ANIM_VERTEX_SIZE, BUFFER_OFFSET(sizeof(float) * 5));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDJoints); // Bind the buffer ID as an ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, perVertexJointData.size() * VERTEX_JOINT_DATA_SIZE, perVertexJointData.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glVertexAttribIPointer(3, 4, GL_INT, VERTEX_JOINT_DATA_SIZE, BUFFER_OFFSET(0)); //Int pointer
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, ANIM_VERTEX_SIZE, BUFFER_OFFSET(sizeof(unsigned int) * 4));
}

void AnimatedModel::VertexJointData::addJointData(unsigned int id, float weight) {
	for (int i = 0; i < JOINTS_PER_VERTEX; i++) {
		if (weights[i] == 0.0f) {
			jointIDs[i] = id;
			weights[i] = weight;
			return;
		}
	}
}

std::string AnimatedModel::getPath() const {
	return this->path;
}

std::string AnimatedModel::getTexturePath() const {
	return this->texturePath;
}

void AnimatedModel::setTexturePath(std::string texPath) {
	this->texturePath = texPath;
}

void AnimatedModel::setVertCount(unsigned int count) {
	this->vertCount = count;
}

unsigned int AnimatedModel::getVertCount() const {
	return this->vertCount;
}

unsigned int AnimatedModel::getTextureID() const {
	return this->textureID;
}

void AnimatedModel::setTextureID(unsigned int id) {
	this->textureID = id;
}

glm::vec3 AnimatedModel::getWorldPosition() const {
	return this->worldPosition;
}

void AnimatedModel::setWorldPosition(glm::vec3 position) {
	this->worldPosition = position;
}

float AnimatedModel::getWorldRotation() const {
	return this->worldRotation;
}

void AnimatedModel::setWorldRotation(float rotation) {
	this->worldRotation = rotation;
}
