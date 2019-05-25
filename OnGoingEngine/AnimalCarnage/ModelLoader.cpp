#include "ModelLoader.h"
#include "System.h"

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}

void ModelLoader::loadAO(GameObject*& object, const char* characterName, int mipLevels) //only for character_
{ 
	if (object->getModel() != nullptr)
		object->getModel().reset();
	//bool modelloaded = false;


	Luna::Reader reader;
	Luna::Skeleton skltn;
	std::vector<Luna::Joint> joints;
	std::vector<Luna::Weights> weights;
	Luna::Animation anims;
	Luna::Material mat;
	Luna::Mesh mesh;
	std::vector<std::vector<Luna::Keyframe>> keyframePack; // pack of all joint - keyframes

	//filename paths
	std::string lu = ".lu";
	std::string animations[2];
	animations[0] = "_idle";
	animations[1] = "_run";
	
	std::string filePath = "";
	for (int i = 0; i < 2; i++) //nrOfAnimations
	{
		filePath = characterName + animations[i] + lu;
		reader.readFile(filePath.c_str());
		mat = reader.getMaterial(0);
		mesh = reader.getMesh(0);
		anims = reader.getAnimation();
		reader.getJoints(joints);
		keyframePack.resize(joints.size());
		skltn = reader.getSkeleton();
		//if (mesh.hasSkeleton==false)
		//	return false;

		shared_ptr<Model> m = System::assetMananger->GetModel(filePath);
		if (m != nullptr)
		{
			//modelloaded = true;

			System::assetMananger->LoadTexture(mat.diffuseTexPath, mat.diffuseTexPath);
			shared_ptr<Texture> texture = System::assetMananger->GetTexture(mat.diffuseTexPath);
			m->SetTexture(texture);

			if (mat.hasGlowMap)
			{
				shared_ptr<Texture> glowmap;
				System::assetMananger->LoadGlowMap(mat.glowTexPath, mat.glowTexPath); //load texture
				glowmap = System::assetMananger->GetTexture(mat.glowTexPath); //set glow texture
				m->setGlowMap(glowmap);
			}

			
			std::string animName(anims.animationName);
	
			if (!object->checkIfAnimExist(animName))
			{
				for (int f = 0; f < joints.size(); f++)
				{
					reader.getKeyframes(f, keyframePack[f]);
				}
				object->setSkeleton(joints);
				object->setNewAnimation(anims.fps, anims.duration, animName, keyframePack);//change to pack
			}
			object->addModel(m, mesh.hasSkeleton); //
			object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);


		}
		else
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
			object->setSkeleton(joints);
			object->setNewAnimation(anims.fps, anims.duration, animName, keyframePack);
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

			shared_ptr<Model> model(new Model());
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
			object->addModel(System::assetMananger->GetModel(filePath), mesh.hasSkeleton); //mesh.hasSkeleton

			if (mesh.hasBoundingBox)
				object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);
			
			vertices3D.clear();
		}

	}
}

void ModelLoader::loadGO(GameObject*& object, const char* filePath, int mipLevels)
{
	if (object->getModel() != nullptr)
		object->getModel().reset();

	
	bool modelloaded = false;
	Luna::Reader reader;
	reader.readFile(filePath);

	
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
			System::assetMananger->LoadGlowMap(mat1.glowTexPath, mat1.glowTexPath); //load texture
			glowmap = System::assetMananger->GetTexture(mat1.glowTexPath); //set glow texture
			m->setGlowMap(glowmap);
		}

		if (mesh.hasSkeleton == true)
		{

			std::vector<Luna::Joint> joints;
			Luna::Animation anims;
			std::vector<std::vector<Luna::Keyframe>> keyframePack; // pack of all joint - keyframes
			reader.getJoints(joints);
			anims = reader.getAnimation();
			keyframePack.resize(joints.size());
			for (int f = 0; f < joints.size(); f++)
			{
				reader.getKeyframes(f, keyframePack[f]);
			}
			std::string animName(anims.animationName);
			object->setSkeleton(joints);
			object->setNewAnimation(anims.fps, anims.duration, animName, keyframePack);//change to pack

		}



		object->addModel(m, mesh.hasSkeleton); //
		object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);
		return;
	}

	



	unsigned int meshCount = reader.getMeshCount();
	std::vector<Luna::Vertex> vertices;
	//std::vector<Luna::Index> indices;
	reader.getVertices(0, vertices);
	//reader.getIndices(0, indices);

	//converting to fit functions and shaders
	std::vector<Vertex3D> vertices3D;
	vertices3D.resize(vertices.size());
	for (int vrt = 0; vrt < vertices.size(); vrt++)
	{
		vertices3D[vrt] = vertices[vrt];
		vertices3D[vrt].uv.y = abs(1 - vertices3D[vrt].uv.y);
	}

	//DWORD* dIndices = new DWORD[indices.size()];
	//for (int j = 0; j < indices.size(); j++)
	//		dIndices[j] = indices[j].vertIndex;

	
	//object = new GameObject;// *[meshCount];

	Luna::Mesh mesh= reader.getMesh(0);
	Luna::Material mat = reader.getMaterial(0);

	
	if (mesh.hasSkeleton == true)
	{
		Luna::Skeleton skltn;
		std::vector<Luna::Joint> joints;
		std::vector<Luna::Weights> weights;
		Luna::Animation anims;
		std::vector<std::vector<Luna::Keyframe>> keyframePack; // pack of all joint - keyframes

		skltn = reader.getSkeleton();
		reader.getWeights(mesh.id, weights);
		reader.getJoints(joints);
		anims = reader.getAnimation();
		keyframePack.resize(joints.size());
		for (int k = 0; k < joints.size(); k++)
		{
			reader.getKeyframes(k, keyframePack[k]);
		}


		std::string animName(anims.animationName);



		object->setSkeleton(joints);
		object->setNewAnimation(anims.fps, anims.duration, animName, keyframePack);//change to pack

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


	}


	
			//check if model is loaded
	if (modelloaded)
		return;

	shared_ptr<Model> model(new Model());
	shared_ptr<Texture> texture;
	model->setMesh(vertices3D);//, dIndices, (int)indices.size());
		
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
	object->addModel(System::assetMananger->GetModel(filePath), mesh.hasSkeleton); //mesh.hasSkeleton

	if (mesh.hasBoundingBox)
		object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);


	vertices3D.clear();
	//delete[]dIndices;

	
}

std::string ModelLoader::getPath() const
{
	return this->path;
}
