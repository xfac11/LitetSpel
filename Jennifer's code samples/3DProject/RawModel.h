#pragma once
#include "glm/glm.hpp"
#include <vector>

#define VERTEX_SIZE sizeof(RawModel::TriangleVertex)

class RawModel {
#define BUFFER_OFFSET(i) ((char *)nullptr + (i)) //To avoid redefinition in main
public:
	// this is how we will structure the input data for the vertex shader
	struct TriangleVertex {
		glm::vec3 positions;
		glm::vec2 UVs;
		glm::vec3 normals;
	};

	RawModel(const char* filePath);
	~RawModel();

	int getVertCount() const;
	void setVertCount(int count);
	const char* getPath() const;
	std::string getTexturePath() const;
	std::string getNormalTexturePath() const;
	void setTexturePath(std::string texPath);
	void setNormalTexturePath(std::string texPath);
	void setMaterialPath(std::string matPath);
	std::string getMaterialPath() const;
	unsigned int getTextureID() const;
	void setTextureID(unsigned int id);
	unsigned int getNormalID() const;
	void setNormalID(unsigned int id);

	glm::vec3 getWorldPosition() const;
	void setWorldPosition(glm::vec3 position);
	float getWorldRotation() const;
	void setWorldRotation(float rotation);


	unsigned int textureID;
	std::vector<TriangleVertex> vertices; //List of all vertices in model file
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned short> vertex_indices, uv_indices, normal_indices;
	//For normal mapping
	std::vector<glm::vec3> tangent;
	std::vector<glm::vec3> bitangent;

	//Used with MTL-files
	glm::vec3 ambientVal;
	glm::vec3 specularVal;
	glm::vec3 diffuseVal;
	unsigned int ambID;
	unsigned int specID;
	unsigned int diffID;

	//Used with blendmapping
	std::string blendmapPath; //File path to blendmap texture file
	std::string rTexPath;
	std::string gTexPath;
	std::string bTexPath;
	unsigned int blendTexID;
	unsigned int rTexID;
	unsigned int gTexID;
	unsigned int bTexID;

	unsigned int vaoID;
	unsigned int vboID;

	void prepare(unsigned int& shaderProgram);
	void createBuffers();
	void prepareBuffers();

	unsigned int shaderProg;

private:
	void prepareMaterials();

	unsigned int normalID;

	const char* path; //File path
	std::string texturePath; //File path to texture file
	std::string normalTexPath;
	std::string materialPath; //File path to .mtl-file
	int vertCount;
	glm::vec3 worldPosition;
	float worldRotation;
};

