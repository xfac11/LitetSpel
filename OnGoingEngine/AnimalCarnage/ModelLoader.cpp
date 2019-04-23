#include "ModelLoader.h"

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}

void ModelLoader::loadModel(Model**&model, const char* filePath)
{
	Luna::Reader reader;
	reader.readFile(filePath);

	unsigned int meshCount = reader.getMeshCount();
	std::vector<Luna::Vertex> vertices;
	std::vector<Luna::Index> indices;
	reader.getVertices(0, vertices);
	reader.getIndices(0, indices);
	/*model = new Model*[meshCount];
	for (int i = 0; i < meshCount; i++)
	{
		model[i] = new Model;
		model[i]->setMesh(vector.mesh, )
		//model[i]->setTexture();
	}*/

	//model.setMesh(vertices, indices, mesh.indexCount);
	
}

std::string ModelLoader::getPath() const
{
	return this->path;
}
