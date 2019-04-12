#pragma once
#include "Luna.h"
#include <vector>

namespace Luna {
	class MeshData {
	public:
		MeshData();
		~MeshData();

		void setID(unsigned int id);
		unsigned int getID() const;
		std::vector<Vertex> vertices;

	private:
		unsigned int id;
		Mesh mesh;
	};
}

