#include "ModelLoader.h"
#include "System.h"

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}
void ModelLoader::loadGO(GameObject*& object, const char* filePath, int mipLevels)
{
	bool modelloaded = false;
	Luna::Reader reader;
	reader.readFile(filePath);

	{
		//check if model is already loaded
		shared_ptr<Model> m = System::assetMananger->GetModel(filePath);
		if (m != nullptr)
		{
			modelloaded = true;
			Luna::Material mat1 = reader.getMaterial(0);
			Luna::Mesh  mesh = reader.getMesh(0);
			System::assetMananger->LoadTexture(mat1.diffuseTexPath, mat1.diffuseTexPath);
			shared_ptr<Texture> texture = System::assetMananger->GetTexture(mat1.diffuseTexPath);
			m->SetTexture(texture);

			if (mat1.hasGlowMap)
			{
				shared_ptr<Texture> glowmap;
				System::assetMananger->LoadTexture(mat1.glowTexPath, mat1.glowTexPath); //load texture
				glowmap = System::assetMananger->GetTexture(mat1.glowTexPath); //set glow texture
				m->setGlowMap(glowmap);
			}

			object->addModel(m, mesh.hasSkeleton); //mesh.hasSkeleton
			object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);
			return;
		}

	}



	unsigned int meshCount = reader.getMeshCount();
	std::vector<Luna::Vertex> vertices;
	std::vector<Luna::Index> indices;
	reader.getVertices(0, vertices);
	reader.getIndices(0, indices);

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
		Luna::Material mat = reader.getMaterial(i);

	
		if (mesh.hasSkeleton == true)
		{
			Luna::Skeleton skltn; //done
			std::vector<Luna::Joint> joints;
			std::vector<Luna::Weights> weights; //done
			Luna::Animation anims;
			//std::vector<Luna::Keyframe> keyframes;  //one joint-  keyframes
			std::vector<std::vector<Luna::Keyframe>> keyframePack; // pack of all joint - keyframes

			skltn = reader.getSkeleton();
			reader.getWeights(mesh.id, weights);
			reader.getJoints(joints);
			anims = reader.getAnimation();
			keyframePack.resize(joints.size());
			for (int i = 0; i < joints.size(); i++)
			{
				reader.getKeyframes(i, keyframePack[i]);
			}

			
			std::string animName(anims.animationName);
			

			
			object[i].setSkeleton(joints);
			object[i].setNewAnimation(anims.fps,anims.duration,animName, keyframePack);//change to pack

			for (int j = 0; j < weights.size(); j++) {
				vertices3D[j].Joint.x = weights[j].jointIDs[0];
				vertices3D[j].Joint.y = weights[j].jointIDs[1];
				vertices3D[j].Joint.z = weights[j].jointIDs[2];
				vertices3D[j].Joint.w = weights[j].jointIDs[3];

				vertices3D[j].Weights.x = weights[j].weights[0];
				vertices3D[j].Weights.y = weights[j].weights[1];
				vertices3D[j].Weights.z = weights[j].weights[2];
				vertices3D[j].Weights.w = weights[j].weights[3];

			}


		}


		{
			//check if model is loaded
			if (modelloaded)
				return;

			shared_ptr<Model> model(new Model());
			shared_ptr<Texture> texture;
			model->setMesh(vertices3D, dIndices, (int)indices.size());
		
			System::assetMananger->LoadTexture(mat.diffuseTexPath, mat.diffuseTexPath); //load texture
			texture = System::assetMananger->GetTexture(mat.diffuseTexPath);
			model->SetTexture(texture); 	//set the texture to the model
			if (mat.hasGlowMap) 
			{
				shared_ptr<Texture> glowmap;
				System::assetMananger->LoadTexture(mat.glowTexPath, mat.glowTexPath); //load texture
				glowmap = System::assetMananger->GetTexture(mat.glowTexPath); //set glow texture
				model->setGlowMap(glowmap);
			}
			System::assetMananger->LoadModel(filePath, model); //load model
			object->addModel(System::assetMananger->GetModel(filePath), mesh.hasSkeleton); //mesh.hasSkeleton

			if (mesh.hasBoundingBox)
				object[i].setHalfSize(reader.getBoundingBox(i).halfSize, reader.getBoundingBox(i).pos);

		}

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
