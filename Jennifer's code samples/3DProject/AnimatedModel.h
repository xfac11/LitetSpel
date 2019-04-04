#pragma once
#include <glm/glm.hpp>
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <vector>
#include <map>

#define JOINTS_PER_VERTEX 4
#define MAX_JOINTS 50
#define ANIM_VERTEX_SIZE sizeof(AnimatedModel::Vertex)
#define VERTEX_JOINT_DATA_SIZE sizeof(AnimatedModel::VertexJointData)

class AnimatedModel {
#define BUFFER_OFFSET(i) ((char *)nullptr + (i)) //To avoid redefinition in main
public:
	struct Vertex {
		glm::vec3 positions;
		glm::vec2 UVs;
		glm::vec3 normals;
	};

	struct VertexJointData {
		unsigned int jointIDs[JOINTS_PER_VERTEX];
		float weights[JOINTS_PER_VERTEX];

		VertexJointData::VertexJointData() { //Constructor
			memset(jointIDs, 0, sizeof(jointIDs)); //Initialize all values in array to 0
			memset(weights, 0, sizeof(weights)); //Initialize all values in array to 0
		};

		void addJointData(unsigned int id, float weight);
	};

	struct JointMatrix {
		aiMatrix4x4 offsetMatrix; //Matrix that describes the transformation from mesh space to local space
		aiMatrix4x4 finalTransform;
	};

	struct Animation {
		float ticksPerSecond = 0.0f;
		double duration = 0.0f;
	};

	AnimatedModel(std::string filePath);
	~AnimatedModel();

	void createBuffers();
	void prepare(unsigned int& shaderProgram);
	void prepareBuffers();

	std::string getPath() const;
	std::string getTexturePath() const;
	void setTexturePath(std::string texPath);
	void setVertCount(unsigned int count);
	unsigned int getVertCount() const;
	unsigned int getTextureID() const;
	void setTextureID(unsigned int id);

	std::vector<Vertex> vertices; //List of all vertices in model
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;

	std::vector<VertexJointData> perVertexJointData;
	std::vector<JointMatrix> joint_matrices;
	std::vector<aiMatrix4x4> jointTransforms; //List of transforms for all joints to send to the vertex shader

	glm::vec3 getWorldPosition() const;
	void setWorldPosition(glm::vec3 position);
	float getWorldRotation() const;
	void setWorldRotation(float rotation);

	glm::vec3 ambientVal, specularVal, diffuseVal;

	unsigned int numIndices;
	unsigned int ambID, specID, diffID;
	unsigned int jointCount;

	unsigned int vaoID;
	unsigned int vboID;
	unsigned int iboID; //Index buffer
	unsigned int vboIDJoints;

	Animation animation; //One animation sequence
	aiMatrix4x4 globalInverseTransform;
	std::map<std::string, unsigned int> joint_mapping; //Maps a joint name and their index
	unsigned int jointLocations[MAX_JOINTS];

	Assimp::Importer importer;
	const aiScene* scene;

	unsigned int shaderProg;

private:
	void prepareMaterials();
	void prepareJoints();

	std::string path; //File path
	std::string texturePath; //File path to texture file
	unsigned int vertCount;

	unsigned int textureID;

	glm::vec3 worldPosition;
	float worldRotation;
};

