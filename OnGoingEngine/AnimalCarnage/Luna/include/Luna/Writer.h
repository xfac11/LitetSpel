#pragma once
#include "Luna.h"
#include <fstream>
#include <vector>

namespace Luna {
	class Writer {
	public:
		Writer();
		~Writer();

		void setVertexPosition(Vertex& vertex, float x, float y, float z);
		void setVertexNormal(Vertex& vertex, float x, float y, float z);
		void setVertexUV(Vertex& vertex, float x, float y);
		void setVertexTangent(Vertex& vertex, float x, float y, float z);
		void setVertexBiTangent(Vertex& vertex, float x, float y, float z);

		void setMaterialAmbient(Material& mat, float r, float g, float b);
		void setMaterialDiffuse(Material& mat, float r, float g, float b);
		void setMaterialSpecular(Material& mat, float r, float g, float b);

		void write(std::ofstream& outfile, Scene& scene);
		void write(std::ofstream& outfile, Mesh& mesh);
		void write(std::ofstream& outfile, Vertex& vertex);
		void write(std::ofstream& outfile, Index& index);
		void write(std::ofstream& outfile, Material& mat);
		void write(std::ofstream& outfile, Weights& weights);
		void write(std::ofstream& outfile, Skeleton& skel);
		void write(std::ofstream& outfile, Joint& joint);
		void write(std::ofstream& outfile, Animation& anim);
		void write(std::ofstream& outfile, Keyframe& frame);
		void write(std::ofstream& outfile, BoundingBox& boundingBox);

		Scene scene;
		Skeleton skeleton;
		Animation animation;
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		std::vector<BoundingBox> boundingBoxes;

	private:
	
	};
}

