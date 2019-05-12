#pragma once

namespace Luna {

#define NAME_SIZE 64
#define PATH_SIZE 128

	struct Scene {
		unsigned int meshCount = 0;
		unsigned int materialCount = 0;
	};

	struct Mesh {
		char name[NAME_SIZE] = "\0";
		unsigned int id = 0;
		float pos[3] = { 0.0f };
		unsigned int vertexCount = 0;
		unsigned int indexCount = 0;
		bool hasSkeleton = false;
		bool hasBoundingBox = false;
	};

	struct Vertex {
		float position[3] = { 0.0f };
		float uv[2] = { 0.0f };
		float normal[3] = { 0.0f };
		float tangent[3] = { 0.0f };
		float bitangent[3] = { 0.0f };

		bool operator==(const Vertex& right) const {
			if (position[0] == right.position[0] &&
				position[1] == right.position[1] &&
				position[2] == right.position[2] &&
				normal[0] == right.normal[0] &&
				normal[1] == right.normal[1] &&
				normal[2] == right.normal[2] &&
				uv[0] == right.uv[0] &&
				uv[1] == right.uv[1] &&
				tangent[0] == right.tangent[0] &&
				tangent[1] == right.tangent[1] &&
				tangent[2] == right.tangent[2] &&
				bitangent[0] == right.bitangent[0] &&
				bitangent[1] == right.bitangent[1] &&
				bitangent[2] == right.bitangent[2]) {
				return true;
			}
			else {
				return false;
			}
		}
	};

	struct Index {
		unsigned int vertIndex = 0;
	};

	struct Material {
		float diffuseVal[3] = { 0.0f };
		float ambientVal[3] = { 0.0f };
		float specularVal[3] = { 0.0f };
		char diffuseTexPath[PATH_SIZE] = "\0";
		char normalTexPath[PATH_SIZE] = "\0";
		char glowTexPath[PATH_SIZE] = "\0";
		bool hasNormalMap = false;
		bool hasGlowMap = false;
	};

	struct Weights {
		unsigned int jointIDs[4] = { 0 };
		float weights[4] = { 0.0f };
	};

	struct Skeleton {
		unsigned int jointCount = 0;
	};

	struct Joint {
		char jointName[NAME_SIZE] = "\0";
		unsigned int jointID = 0;
		int parentID = -1;
		float invBindposeMatrix[4][4] = { 0.0f };
	};

	struct Animation {
		float fps = 0.0f;
		float duration = 0.0f;
		char animationName[NAME_SIZE] = "\0";
		unsigned int keyframeCount = 0;
	};

	struct Keyframe {
		float timePosition = 0.0f;
		float translation[4] = { 0.0f };
		float rotation[4] = { 0.0f };
		float scale[4] = { 0.0f };
	};

	struct BoundingBox {
		float pos[3] = { 0.0f };
		float halfSize[3] = { 0.0f };
	};
}