#include "ModelLoader.h"

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}
void ModelLoader::loadGO(GameObject*& object, const char* filePath)
{
	Luna::Reader reader;
	reader.readFile(filePath);

	unsigned int meshCount = reader.getMeshCount();
	std::vector<Luna::Vertex> vertices;
	std::vector<Luna::Index> indices;
	reader.getVertices(0, vertices);
	reader.getIndices(0, indices);

	//int skltn = (int)reader.getSkeletonCount();
	//std::vector<Luna::Joint> joints;
	//std::vector<Luna::Weights> weights;
	//std::vector<Luna::Animation> anims;
	//reader.getWeights(0, weights);
	//reader.getAnimation();
	//converting to fit functions and shaders
	std::vector<Vertex3D> vertices3D;
	vertices3D.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices3D[i] = vertices[i];
		vertices3D[i].uv.y = abs(1 - vertices3D[i].uv.y);
	}

	DWORD* dIndices = new DWORD[indices.size()];
	for (int j = 0; j < indices.size(); j++)
			dIndices[j] = indices[j].vertIndex;

	
	//object = new GameObject;// *[meshCount];
	for (int i = 0; i < (int)meshCount; i++)
	{
		Luna::Mesh mesh= reader.getMesh(i);
		//object[i] = new GameObject;
		if(mesh.hasBoundingBox)
			object[i].setHalfSize(reader.getBoundingBox(i).halfSize, reader.getBoundingBox(i).pos);
		object[i].addModel(vertices3D, dIndices, (int)indices.size());
		object[i].setTexture(reader.getMaterial(i).diffuseTexPath,i);
		//reader.getMaterial(i).
	}

	vertices3D.clear();
	delete[]dIndices;
}
//void ModelLoader::loadModel(Model**&model, const char* filePath) //unused?
//{
//	Luna::Reader reader;
//	reader.readFile(filePath);
//
//	unsigned int meshCount = reader.getMeshCount();
//	std::vector<Luna::Vertex> vertices;
//	std::vector<Luna::Index> indices;
//	reader.getVertices(0, vertices);
//	reader.getIndices(0, indices);
//
//
//	//converting to fit functions and shaders
//	std::vector<Vertex3D> vertices3D; 
//	vertices3D.resize(vertices.size());
//	for (int i = 0; i < vertices.size(); i++)
//		vertices3D[i] = vertices[i];
//	DWORD* dIndices =new DWORD[indices.size()];
//	for (int j = 0; j < indices.size(); j++)
//		dIndices[j] = indices[j].vertIndex;
//
//	model = new Model*[meshCount];
//	for (int i = 0; i < (int)meshCount; i++)
//	{
//		model[i] = new Model;
//		model[i]->setMesh(vertices3D, dIndices, 0);
//		model[i]->setTexture("Textures/cat.tga");
//		model[i]->setSampler();
//
//	}
//
//}

std::string ModelLoader::getPath() const
{
	return this->path;
}
