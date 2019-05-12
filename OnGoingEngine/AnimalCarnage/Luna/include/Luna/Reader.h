#pragma once
#include "Luna.h"
#include <fstream>
#include <vector>

namespace Luna {
	class Reader {
	public:
		Reader();
		~Reader();

		void readFile(const char* filePath); //Entry point

		unsigned int getMeshCount() const;
		unsigned int getMaterialCount() const;

		Mesh getMesh(int id) const;
		Skeleton getSkeleton() const;
		Animation getAnimation() const;

		Material getMaterial(int meshID) const;
		BoundingBox getBoundingBox(int meshID) const;

		void getVertices(int meshID, std::vector<Vertex>& vertices);
		void getIndices(int meshID, std::vector<Index>& indices);
		void getWeights(int meshID, std::vector<Weights>& weights); //The number of weights is equal to the number of vertices
		void getKeyframes(int jointID, std::vector<Keyframe>& keyframes);
		void getMaterials(std::vector<Material>& materials);
		void getJoints(std::vector<Joint>& joints);

		bool animationExist() const;

	private:
		void read(std::ifstream& infile, Scene& scene);
		void read(std::ifstream& infile, Mesh& mesh);
		void read(std::ifstream& infile, Vertex& vertex);
		void read(std::ifstream& infile, Index& index);
		void read(std::ifstream& infile, Material& mat);
		void read(std::ifstream& infile, Weights& weights);
		void read(std::ifstream& infile, Skeleton& skeleton);
		void read(std::ifstream& infile, Joint& joint);
		void read(std::ifstream& infile, Animation& anim);
		void read(std::ifstream& infile, Keyframe& frame);
		void read(std::ifstream& infile, BoundingBox& boundingBox);

		void clean();

		Scene scene;
		Skeleton skeleton;
		Animation animation;
		std::vector<Mesh> meshes;
		std::vector<Joint> joints;
		std::vector<Material> materials;
		std::vector<BoundingBox> boundingBoxes;
		std::vector<Vertex*> meshVertices;
		std::vector<Index*> meshIndices;
		std::vector<Weights*> weights;
		std::vector<Keyframe*> keyframes;

		unsigned int meshCount;
		unsigned int materialCount;
		bool hasAnimation;
	};
}
