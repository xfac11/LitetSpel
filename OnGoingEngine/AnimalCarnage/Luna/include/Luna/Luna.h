#pragma once

namespace Luna {

#define NAME_SIZE 64
#define PATH_SIZE 128

	//struct Scene {
	//	unsigned int modelCount;
	//};

	struct Scene {
		unsigned int meshCount;
		unsigned int skeletonCount;
		unsigned int materialCount;
	};

	struct Mesh {
		char name[NAME_SIZE];
		unsigned int id;
		float pos[3];
		unsigned int vertexCount;
		unsigned int indexCount;
		unsigned int materialID;
		bool hasSkeleton = false;
		bool hasBoundingBox = false;
	};

	struct Vertex {
		float position[3];
		float uv[2];
		float normal[3];
		float tangent[3];
		float bitangent[3];
	};

	struct Index {
		unsigned int vertIndex;
	};

	struct Material {
		unsigned int index;
		float diffuseVal[3];
		float ambientVal[3];
		float specularVal[3];
		char diffuseTexPath[PATH_SIZE];
		bool hasNormalMap = false;
	};

	struct NormalMap {
		char normalTexPath[PATH_SIZE];
	};

	struct Weights {
		unsigned int jointIDs[4];
		float weights[4];
	};

	struct Skeleton {
		unsigned int skeletonID;
		unsigned int jointCount;
		unsigned int animationCount;
	};

	struct Joint {
		char jointName[NAME_SIZE];
		unsigned int jointID;;
		unsigned int parentID;
		float bindposeMatrix[4][4];
	};

	struct Animation {
		float fps;
		float duration;
		char animationName[NAME_SIZE];
		unsigned int animationID;
		unsigned int keyframeCount;
	};

	struct Keyframe {
		float timePosition;
		float keyframeMatrix[4][4];
	};

	struct BoundingBox {
		float pos[3];
		float halfSize[3];
	};
}