#pragma once
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>
#include "AnimatedModel.h"
#include <vector>
#include <map>

class AssimpLoader {
public:

	AssimpLoader();
	~AssimpLoader();

	//bool LoadModel(AnimatedModel &model);
	bool LoadModel(AnimatedModel* model);
	void CalcJointTransform(double timeInSeconds, AnimatedModel* model);
	glm::mat4 aiToMat4(aiMatrix4x4 matrix); //Converts a aiMatrix4x4 to glm::mat4

private:
	void ReadNodeHeirarchy(float animation_time, const aiNode* node, const aiMatrix4x4 parentTransform, AnimatedModel* model);

	const aiNodeAnim* FindNodeAnim(const aiAnimation* anim, const std::string nodeName);
	unsigned int FindRotation(float animTime, const aiNodeAnim* animation);
	unsigned int FindScaling(float animTime, const aiNodeAnim* animation);
	unsigned int FindPosition(float animTime, const aiNodeAnim* animation);

	aiVector3D CalcInterpolatedScaling(float animationTime, const aiNodeAnim* anim);
	aiQuaternion CalcInterpolatedRotation(float animationTime, const aiNodeAnim* anim);
	aiVector3D CalcInterpolatedTranslation(float animationTime, const aiNodeAnim* anim);
};

