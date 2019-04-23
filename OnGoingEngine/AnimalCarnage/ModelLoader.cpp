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



	//converting to fit functions and shaders
	//std::vector<Vertex3D> vertices3D; 
	//for (int i = 0; i < vertices.size(); i++)
	//	vertices3D[i] = vertices[i];
	//DWORD* dIndices =new DWORD[indices.size()];
	//for (int j = 0; j < indices.size(); j++)
	//	dIndices[j] = indices[j].vertIndex;


	//model[0]->setMesh(vertices3D, dIndices, 0);

	/*model = new Model*[meshCount];
	for (int i = 0; i < meshCount; i++)
	{
		model[i] = new Model;
		model[i]->setMesh(vertices3D, in )
		//model[i]->setTexture();
	}*/

	//model[0]->setMesh(vertices, nullptr, 0);
	//model.setMesh(vertices, indices, mesh.indexCount);
	

}

std::string ModelLoader::getPath() const
{
	return this->path;
}
