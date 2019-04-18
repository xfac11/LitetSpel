#pragma once

namespace Luna {

#define NAME_SIZE 64
#define PATH_SIZE 128

	//struct Scene {
	//	unsigned int modelCount;
	//};

	struct Scene {
		unsigned int meshCount = 0;
		unsigned int skeletonCount = 0;
		unsigned int materialCount = 0;
	};

	struct Mesh {
		char name[NAME_SIZE] = "\0";
		unsigned int id = 0;
		float pos[3] = { 0.0f };
		unsigned int vertexCount = 0;
		unsigned int indexCount = 0;
		unsigned int materialID = 0;
		bool hasSkeleton = false;
		bool hasBoundingBox = false;
	};

	struct Vertex {
		float position[3] = { 0.0f };
		float uv[2] = { 0.0f };
		float normal[3] = { 0.0f };
		float tangent[3] = { 0.0f };
		float bitangent[3] = { 0.0f };
	};

	struct Index {
		unsigned int vertIndex = 0;
	};

	struct Material {
		unsigned int index = 0;
		float diffuseVal[3] = { 0.0f };
		float ambientVal[3] = { 0.0f };
		float specularVal[3] = { 0.0f };
		char diffuseTexPath[PATH_SIZE] = "";
		bool hasNormalMap = false;
	};

	struct NormalMap {
		char normalTexPath[PATH_SIZE] = "";
	};

	struct Weights {
		unsigned int jointIDs[4] = { 0 };
		float weights[4] = { 0.0f };
	};

	struct Skeleton {
		unsigned int skeletonID = 0;
		unsigned int jointCount = 0;
		unsigned int animationCount = 0;
	};

	struct Joint {
		char jointName[NAME_SIZE] = "";
		unsigned int jointID = 0;
		unsigned int parentID = 0;
		float bindposeMatrix[4][4];
	};

	struct Animation {
		float fps = 0.0f;
		float duration = 0.0f;
		char animationName[NAME_SIZE] = "";
		unsigned int animationID = 0;
		unsigned int keyframeCount = 0;
	};

	struct Keyframe {
		float timePosition = 0.0f;
		float keyframeMatrix[4][4];
	};

	struct BoundingBox {
		float pos[3] = { 0.0f };
		float halfSize[3] = { 0.0f };
	};
}