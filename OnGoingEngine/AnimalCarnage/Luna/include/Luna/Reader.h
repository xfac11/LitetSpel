#pragma once
#include "Luna.h"
#include "MeshData.h"
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
		unsigned int getSkeletonCount() const;

		void getMeshes(std::vector<Mesh>& meshes);
		Mesh getMesh(int id) const;
		void getVertices(int meshID, std::vector<Vertex>& vertices);
		void getIndices(int meshID, std::vector<Index>& indices);

	private:
		void read(std::ifstream& infile, Scene& scene);
		void read(std::ifstream& infile, Mesh& mesh);
		void read(std::ifstream& infile, Vertex& vertex);
		void read(std::ifstream& infile, Index& index);
		void read(std::ifstream& infile, Material& mat);
		void read(std::ifstream& infile, NormalMap& normalmap);
		void read(std::ifstream& infile, Weights& weights);
		void read(std::ifstream& infile, Skeleton& skeleton);
		void read(std::ifstream& infile, Joint& joint);
		void read(std::ifstream& infile, Animation& anim);
		void read(std::ifstream& infile, Keyframe& frame);
		void read(std::ifstream& infile, BoundingBox& boundingBox);

		void clean();

		Scene scene;
		std::vector<Mesh> meshes;
		std::vector<Vertex*> meshVertices;
		std::vector<Index*> meshIndices;

		//std::vector<MeshData> meshdata;

		unsigned int meshCount;
		unsigned int materialCount;
		unsigned int skeletonCount;
	};
}
