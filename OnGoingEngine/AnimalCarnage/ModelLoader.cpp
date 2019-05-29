#include "ModelLoader.h"
#include "System.h"

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}

void ModelLoader::loadAO(GameObject*& object, const char* characterName, std::vector<std::string> animalAnimations, int mipLevels) //only for character_
{ 
	if (object->getModel() != nullptr)
		object->getModel().reset();

	Luna::Reader reader;
	Luna::Skeleton skltn;
	std::vector<Luna::Joint> joints;
	std::vector<Luna::Weights> weights;
	Luna::Animation anims;
	Luna::Material mat;
	Luna::Mesh mesh;
	std::vector<std::vector<Luna::Keyframe>> keyframePack; // pack of all joint - keyframes

	std::string lu = ".lu";
	std::string initPath = characterName + animalAnimations[0] + lu;
	shared_ptr<Model> model = System::assetMananger->GetModel(initPath);

	std::string filePath = "";
	for (int i = 0; i < animalAnimations.size(); i++) //nrOfAnimations
	{
		filePath = characterName + animalAnimations[i] + lu;
		reader.readFile(filePath.c_str());

		mat = reader.getMaterial(0);
		mesh = reader.getMesh(0);
		anims = reader.getAnimation();
		reader.getJoints(joints);
		keyframePack.resize(joints.size());
		skltn = reader.getSkeleton();

		if (model != nullptr)
		{
			System::assetMananger->LoadTexture(mat.diffuseTexPath, mat.diffuseTexPath);
			shared_ptr<Texture> texture = System::assetMananger->GetTexture(mat.diffuseTexPath);
			model->SetTexture(texture);

			if (mat.hasGlowMap)
			{
				shared_ptr<Texture> glowmap;
				System::assetMananger->LoadGlowMap(mat.glowTexPath, mat.glowTexPath); //load texture
				glowmap = System::assetMananger->GetTexture(mat.glowTexPath); //set glow texture
				model->setGlowMap(glowmap);
			}
			
			std::string animName(anims.animationName);
	
			if (!object->checkIfAnimExist(animName, characterName))
			{
				for (int f = 0; f < joints.size(); f++)
				{
					reader.getKeyframes(f, keyframePack[f]);
				}
				//object->setCurrentAnimal(characterName);
				object->setSkeleton(joints);
				object->setNewAnimation(anims.fps, anims.duration, animName, keyframePack);//change to pack
			}
			//if (mesh.hasBoundingBox)
			//	object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);
			//if (i == 0)
			//	object->addModel(model, mesh.hasSkeleton); //

		}
		else if(model == nullptr)
		{
			std::vector<Luna::Vertex> vertices;
			std::vector<Vertex3D> vertices3D;
			reader.getVertices(0, vertices);
			vertices3D.resize(vertices.size());
			for (int vrt = 0; vrt < vertices.size(); vrt++)
			{
				vertices3D[vrt] = vertices[vrt];
				vertices3D[vrt].uv.y = abs(1 - vertices3D[vrt].uv.y);
			}

			reader.getWeights(mesh.id, weights);
			keyframePack.resize(joints.size());
			for (int k = 0; k < joints.size(); k++)
			{
				reader.getKeyframes(k, keyframePack[k]);
			}

			std::string animName(anims.animationName);

			if (!object->checkIfAnimExist(animName, characterName))
			{
				object->setSkeleton(joints);
				object->setNewAnimation(anims.fps, anims.duration, animName, keyframePack);
			}
			for (int jw = 0; jw < weights.size(); jw++)
			{
				vertices3D[jw].Joint.x = weights[jw].jointIDs[0];
				vertices3D[jw].Joint.y = weights[jw].jointIDs[1];
				vertices3D[jw].Joint.z = weights[jw].jointIDs[2];
				vertices3D[jw].Joint.w = weights[jw].jointIDs[3];

				vertices3D[jw].Weights.x = weights[jw].weights[0];
				vertices3D[jw].Weights.y = weights[jw].weights[1];
				vertices3D[jw].Weights.z = weights[jw].weights[2];
				vertices3D[jw].Weights.w = weights[jw].weights[3];
			}

			model= shared_ptr<Model>(new Model());
			shared_ptr<Texture> texture;
			model->setMesh(vertices3D);

			System::assetMananger->LoadTexture(mat.diffuseTexPath, mat.diffuseTexPath); //load texture
			texture = System::assetMananger->GetTexture(mat.diffuseTexPath);
			model->SetTexture(texture); 	//set the texture to the model
			if (mat.hasGlowMap)
			{
				shared_ptr<Texture> glowmap;

				System::assetMananger->LoadGlowMap(mat.glowTexPath, mat.glowTexPath); //load texture
				glowmap = System::assetMananger->GetTexture(mat.glowTexPath); //set glow texture
				model->setGlowMap(glowmap);
			}
			System::assetMananger->LoadModel(filePath, model); //load model
			//if (mesh.hasBoundingBox)
			//	object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);
			//if(i==0)
			//	object->addModel(System::assetMananger->GetModel(filePath), mesh.hasSkeleton); //mesh.hasSkeleton			
			vertices3D.clear();
		}

	}

	object->addModel(model, mesh.hasSkeleton);
	

	//set half size pushes in a array 
	reader.readFile(initPath.c_str());
	if (mesh.hasBoundingBox)
		object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);
	//if (mesh.hasBoundingBox)
	//	object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);
	//object->addModel(System::assetMananger->GetModel(filePath), mesh.hasSkeleton);
}

void ModelLoader::loadGO(GameObject*& object, const char* filePath, int mipLevels)
{
	if (object->getModel() != nullptr)
		object->getModel().reset();

	Luna::Reader reader;
	Luna::Mesh mesh;
	Luna::Material mat;
	
	reader.readFile(filePath);
	mesh = reader.getMesh(0);
	mat = reader.getMaterial(0);

	shared_ptr<Model> model = System::assetMananger->GetModel(filePath);
	System::assetMananger->LoadTexture(mat.diffuseTexPath, mat.diffuseTexPath);
	shared_ptr<Texture> texture = System::assetMananger->GetTexture(mat.diffuseTexPath);
	shared_ptr<Texture> glowmap;
	if (mat.hasGlowMap)
	{
		System::assetMananger->LoadGlowMap(mat.glowTexPath, mat.glowTexPath); //load texture
		glowmap = System::assetMananger->GetTexture(mat.glowTexPath); //set glow texture
	}

	//check if model is already loaded
	if (model != nullptr)
	{
		model->SetTexture(texture);
		if (mat.hasGlowMap)
			model->setGlowMap(glowmap);
		object->addModel(model, mesh.hasSkeleton); 
	}
	else if (model == nullptr)
	{
		std::vector<Luna::Vertex> vertices;
		std::vector<Vertex3D> vertices3D;
		reader.getVertices(0, vertices);
		vertices3D.resize(vertices.size());
		for (int vrt = 0; vrt < vertices.size(); vrt++) //converting to fit functions and shaders
		{
			vertices3D[vrt] = vertices[vrt];
			vertices3D[vrt].uv.y = abs(1 - vertices3D[vrt].uv.y);
		}

		model = shared_ptr<Model>(new Model());
		model->setMesh(vertices3D);
		model->SetTexture(texture); //set the texture to the model
		if (mat.hasGlowMap)
			model->setGlowMap(glowmap);
		
		System::assetMananger->LoadModel(filePath, model); 
		object->addModel(System::assetMananger->GetModel(filePath), mesh.hasSkeleton); 
		vertices3D.clear(); //
	}
	if (mesh.hasBoundingBox)
		object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);
}

std::string ModelLoader::getPath() const
{
	return this->path;
}
