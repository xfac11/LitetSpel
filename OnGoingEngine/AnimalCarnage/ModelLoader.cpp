#include "ModelLoader.h"

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}
void ModelLoader::loadGO(GameObject*& object, const char* filePath,std::string texture)
{
	Luna::Reader reader;
	reader.readFile(filePath);

	unsigned int meshCount = reader.getMeshCount();
	std::vector<Luna::Vertex> vertices;
	std::vector<Luna::Index> indices;
	reader.getVertices(0, vertices);
	reader.getIndices(0, indices);


	//converting to fit functions and shaders
	std::vector<Vertex3D> vertices3D;
	vertices3D.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
		vertices3D[i] = vertices[i];

	DWORD* dIndices = new DWORD[indices.size()];
	for (int j = 0; j < indices.size(); j++)
			dIndices[j] = indices[j].vertIndex;


	//object = new GameObject;// *[meshCount];
	for (int i = 0; i < (int)meshCount; i++)
	{
		//object[i] = new GameObject;
		object[i].addModel(vertices3D, dIndices, (int)indices.size());
		object[i].setTexture(texture,i);
	}

	vertices3D.clear();
	delete[]dIndices;
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
	std::vector<Vertex3D> vertices3D; 
	vertices3D.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
		vertices3D[i] = vertices[i];
	DWORD* dIndices =new DWORD[indices.size()];
	for (int j = 0; j < indices.size(); j++)
		dIndices[j] = indices[j].vertIndex;

	model = new Model*[meshCount];
	for (int i = 0; i < (int)meshCount; i++)
	{
		model[i] = new Model;
		model[i]->setMesh(vertices3D, dIndices, 0);
		model[i]->setTexture("Textures/cat.tga");
		model[i]->setSampler();

	}

}

std::string ModelLoader::getPath() const
{
	return this->path;
}
