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
	reader.getVertices(0, vertices);
	/*model = new Model*[meshCount];
	for (int i = 0; i < meshCount; i++)
	{
		model[i] = new Model;
		model[i]->setMesh()
		//model[i]->setTexture();
	}*/
	
	//std::vector<Luna::Vertex> vertices;

	//model.setMesh(vertices, indices, mesh.indexCount);
	
}

std::string ModelLoader::getPath() const
{
	return this->path;
}
