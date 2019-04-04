#include "AssimpLoader.h"
#include "glfw/include/GLFW/glfw3.h"
#include <windows.h>
#include <string>

AssimpLoader::AssimpLoader() {

}

AssimpLoader::~AssimpLoader() {
	OutputDebugStringA("Destructor is run for AssimpLoader\n");
}

bool AssimpLoader::LoadModel(AnimatedModel* model) {
	model->scene = model->importer.ReadFile(model->getPath(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_LimitBoneWeights | aiProcess_JoinIdenticalVertices); //Path and post processing flags
	if (model->scene != nullptr) {
		model->globalInverseTransform = model->scene->mRootNode->mTransformation; //Get the global transform and then inverse it
		model->globalInverseTransform = model->globalInverseTransform.Inverse();

		assert(model->scene->mAnimations != nullptr); //Make sure the loaded scene actually has animation

		if (model->scene->mAnimations[0]->mTicksPerSecond != 0.0f) { //Get the framerate for the animation
			model->animation.ticksPerSecond = model->scene->mAnimations[0]->mTicksPerSecond;
			model->animation.duration = model->scene->mAnimations[0]->mDuration;
		}
		else {
			model->animation.ticksPerSecond = 30.0f; //If no framerate is found, default to 30
		}

		AnimatedModel::Vertex temp; //Temporary vertex for storing data

		for (unsigned int i = 0; i < model->scene->mNumMeshes; i++) { //Right now, all mesh data in the loaded scene is stored in one single animated model
			const aiMesh* mesh = model->scene->mMeshes[i];
			const aiMaterial* material = model->scene->mMaterials[mesh->mMaterialIndex];

			//Joint data
			model->perVertexJointData.resize(mesh->mNumVertices);
			aiBone** bones = mesh->mBones;
			for (int j = 0; j < mesh->mNumBones; j++) {
				unsigned int jointID = 0;
				std::string jointName = bones[j]->mName.data;

				if (model->joint_mapping.find(jointName) == model->joint_mapping.end()) { //Looks for the joint name and returns end() if it can't find it
					//Create a new joint and allocate an index to it
					jointID = model->jointCount;
					model->jointCount++;
					AnimatedModel::JointMatrix matrix;
					model->joint_matrices.push_back(matrix);
					model->joint_matrices[jointID].offsetMatrix = bones[j]->mOffsetMatrix;
					model->joint_mapping[jointName] = jointID;
				}
				else {
					jointID = model->joint_mapping[jointName];
				}

				for (int k = 0; k < mesh->mBones[j]->mNumWeights; k++) {
					unsigned int vertexID = mesh->mBones[j]->mWeights[k].mVertexId;
					float weight = mesh->mBones[j]->mWeights[k].mWeight;
					model->perVertexJointData[vertexID].addJointData(jointID, weight);
				}
			}

			//Get material values
			aiColor3D diff, amb, spec;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
			material->Get(AI_MATKEY_COLOR_AMBIENT, amb);
			material->Get(AI_MATKEY_COLOR_SPECULAR, spec);
			model->diffuseVal = glm::vec3(diff.r, diff.g, diff.b);
			model->ambientVal = glm::vec3(amb.r, amb.g, amb.b);
			model->specularVal = glm::vec3(spec.r, spec.g, spec.b);

			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL);
			std::string fileName = path.data;
			std::string fullPath = "Resources/Textures/" + fileName;
			model->setTexturePath(fullPath);

			for (int j = 0; j < mesh->mNumVertices; j++) {
				model->positions.push_back(glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z));
				model->normals.push_back(glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z));
				model->uvs.push_back(glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)); //Only first set of texture coordinates are relevant in this case

				temp.positions = model->positions[j];
				temp.UVs = model->uvs[j];
				temp.normals = model->normals[j];

				model->vertices.push_back(temp); //Push the vertex to list of vertices
			}

			for (int j = 0; j < mesh->mNumFaces; j++) {
				if (mesh->mFaces[j].mNumIndices == 3) {
					model->indices.push_back(mesh->mFaces[j].mIndices[0]);
					model->indices.push_back(mesh->mFaces[j].mIndices[1]);
					model->indices.push_back(mesh->mFaces[j].mIndices[2]);
				}
				else {
					OutputDebugStringA("ASSIMP: Model-loading failure! Mesh is not triangulated.");
					return false;
				}
			}
			model->numIndices = model->indices.size();
			model->setVertCount(model->vertices.size());
		}
	}
	else {
		OutputDebugStringA(model->importer.GetErrorString());
		return false;
	}

	return true;
}

glm::mat4 AssimpLoader::aiToMat4(aiMatrix4x4 matrix) {
	glm::mat4 convertedMat;
	convertedMat[0][0] = matrix.a1;
	convertedMat[1][0] = matrix.b1;
	convertedMat[2][0] = matrix.c1;
	convertedMat[3][0] = matrix.d1;

	convertedMat[0][1] = matrix.a2;
	convertedMat[1][1] = matrix.b2;
	convertedMat[2][1] = matrix.c2;
	convertedMat[3][1] = matrix.d2;

	convertedMat[0][2] = matrix.a3;
	convertedMat[1][2] = matrix.b3;
	convertedMat[2][2] = matrix.c3;
	convertedMat[3][2] = matrix.d3;

	convertedMat[0][3] = matrix.a4;
	convertedMat[1][3] = matrix.b4;
	convertedMat[2][3] = matrix.c4;
	convertedMat[3][3] = matrix.d4;
	return convertedMat;
}

void AssimpLoader::CalcJointTransform(double timeInSeconds, AnimatedModel* model) {
	aiMatrix4x4 identity;

	double timeInTicks = timeInSeconds * model->animation.ticksPerSecond;
	float animationTime = fmod(timeInTicks, model->animation.duration);

	ReadNodeHeirarchy(animationTime, model->scene->mRootNode, identity, model);

	model->jointTransforms.resize(model->jointCount);

	for (int i = 0; i < model->jointCount; i++) {
		model->jointTransforms[i] = model->joint_matrices[i].finalTransform; //Converts matrix to glm::mat4 and places it in list
	}
}

// This is a recursive function that calculates the final transformation for all of the joints in the whole skeleton, starting from the root joint //
void AssimpLoader::ReadNodeHeirarchy(float animation_time, const aiNode* node, const aiMatrix4x4 parentTransform, AnimatedModel* model) {
	std::string nodeName = node->mName.data;
	const aiAnimation* animation = model->scene->mAnimations[0]; //Only supports one animation sequence
	aiMatrix4x4 nodeTransform = node->mTransformation;
	const aiNodeAnim* nodeAnimation = FindNodeAnim(animation, nodeName);

	if (nodeAnimation) {
		//Calculate scaling
		aiVector3D scalingVector = CalcInterpolatedScaling(animation_time, nodeAnimation);
		aiMatrix4x4 scalingMatrix;
		aiMatrix4x4::Scaling(scalingVector, scalingMatrix);

		//Calculate rotation
		aiQuaternion rotationQuaternion = CalcInterpolatedRotation(animation_time, nodeAnimation);
		aiMatrix4x4 rotationMatrix = aiMatrix4x4(rotationQuaternion.GetMatrix());

		//Calculate translation
		aiVector3D translationVector = CalcInterpolatedTranslation(animation_time, nodeAnimation);
		aiMatrix4x4 translationMatrix;
		aiMatrix4x4::Translation(translationVector, translationMatrix);

		//Combine the transformations
		nodeTransform = translationMatrix * rotationMatrix * scalingMatrix;
	}

	aiMatrix4x4 globalTransform = parentTransform * nodeTransform;

	if (model->joint_mapping.find(nodeName) != model->joint_mapping.end()) { //If the node name exists in the map
		unsigned int jointIndex = model->joint_mapping[nodeName];
		model->joint_matrices[jointIndex].finalTransform = model->globalInverseTransform * globalTransform * model->joint_matrices[jointIndex].offsetMatrix;
	}

	for (int i = 0; i < node->mNumChildren; i++) { //Calculate the information for all children joints
		ReadNodeHeirarchy(animation_time, node->mChildren[i], globalTransform, model);
	}
}

const aiNodeAnim* AssimpLoader::FindNodeAnim(const aiAnimation* anim, const std::string nodeName) {
	for (int i = 0; i < anim->mNumChannels; i++) {
		const aiNodeAnim* nodeAnimation = anim->mChannels[i];
		if (std::string(nodeAnimation->mNodeName.data) == nodeName) {
			return nodeAnimation;
		}
	}
	return NULL; //If returning NULL, there was no animation that matched the node name
}

unsigned int AssimpLoader::FindRotation(float animTime, const aiNodeAnim* animation) {
	//Finds the rotation key that is immediately before the animation time provided
	for (int i = 0; i < animation->mNumRotationKeys - 1; i++) {
		if (animTime < (float)animation->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0); //Execution should never get here. Can't find any rotation keys.
	return 0;
}

unsigned int AssimpLoader::FindScaling(float animTime, const aiNodeAnim* animation) {
	//Finds the scaling key that is immediately before the animation time provided
	for (int i = 0; i < animation->mNumScalingKeys - 1; i++) {
		if (animTime < (float)animation->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0); //Execution should never get here. Can't find any scaling keys.
	return 0;
}

unsigned int AssimpLoader::FindPosition(float animTime, const aiNodeAnim* animation) {
	//Finds the position key that is immediately before the animation time provided
	for (int i = 0; i < animation->mNumPositionKeys - 1; i++) {
		if (animTime < (float)animation->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0); //Execution should never get here. Can't find any position keys.
	return 0;
}

aiVector3D AssimpLoader::CalcInterpolatedScaling(float animationTime, const aiNodeAnim* anim) {
	//Need two values to interpolate
	if (anim->mNumScalingKeys == 1) {
		return anim->mScalingKeys[0].mValue;
	}

	unsigned int scalingIndex = FindScaling(animationTime, anim);
	unsigned int nextScalingIndex = scalingIndex + 1;

	assert(nextScalingIndex < anim->mNumScalingKeys); //Make sure to not go outside the number of available keys

	float deltaTime = anim->mScalingKeys[nextScalingIndex].mTime - anim->mScalingKeys[scalingIndex].mTime;
	float factor = (animationTime - (float)anim->mScalingKeys[scalingIndex].mTime) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f); //Make sure the factor is between 0 and 1

	aiVector3D start = anim->mScalingKeys[scalingIndex].mValue;
	aiVector3D end = anim->mScalingKeys[nextScalingIndex].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

aiQuaternion AssimpLoader::CalcInterpolatedRotation(float animationTime, const aiNodeAnim* anim) {
	//Need two values to interpolate
	if (anim->mNumRotationKeys == 1) {
		return anim->mRotationKeys[0].mValue;
	}

	unsigned int rotationIndex = FindRotation(animationTime, anim);
	unsigned int nextRotationIndex = rotationIndex + 1;

	assert(nextRotationIndex < anim->mNumRotationKeys); //Make sure to not go outside the number of available keys

	float deltaTime = anim->mRotationKeys[nextRotationIndex].mTime - anim->mRotationKeys[rotationIndex].mTime;
	float factor = (animationTime - (float)anim->mRotationKeys[rotationIndex].mTime) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f); //Make sure the factor is between 0 and 1

	const aiQuaternion startRotationQ = anim->mRotationKeys[rotationIndex].mValue;
	const aiQuaternion endRotationQ = anim->mRotationKeys[nextRotationIndex].mValue;

	aiQuaternion interpolatedQ;
	aiQuaternion::Interpolate(interpolatedQ, startRotationQ, endRotationQ, factor); //Performs slerp
	interpolatedQ = interpolatedQ.Normalize();

	return interpolatedQ;
}

aiVector3D AssimpLoader::CalcInterpolatedTranslation(float animationTime, const aiNodeAnim* anim) {
	//Need two values to interpolate
	if (anim->mNumPositionKeys == 1) {
		return anim->mPositionKeys[0].mValue;
	}

	unsigned int positionIndex = FindPosition(animationTime, anim);
	unsigned int nextPositionIndex = positionIndex + 1;

	assert(nextPositionIndex < anim->mNumPositionKeys); //Make sure to not go outside the number of available keys

	float deltaTime = anim->mPositionKeys[nextPositionIndex].mTime - anim->mPositionKeys[positionIndex].mTime;
	float factor = (animationTime - (float)anim->mPositionKeys[positionIndex].mTime) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f); //Make sure the factor is between 0 and 1

	aiVector3D start = anim->mPositionKeys[positionIndex].mValue;
	aiVector3D end = anim->mPositionKeys[nextPositionIndex].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}
