#include "ModelLoader.h"
#include "System.h"

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}

void ModelLoader::loadAO(GameObject*& object, const char* characterName, std::vector<std::string> & animalAnimations, std::string attackJoint, int mipLevels) //only for character_
{ 
	if (object->getModel() != nullptr)
		object->getModel().reset();
	
	if (characterName == "Resources/Models/bear_character") {
		object->setScale(0.15, 0.15, 0.15);
		//object->setRotation(0, 0, 1, 3.14 / 2);
	}
	else {
		object->setScale(1, 1, 1);
	}

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
	std::string filePath = "";
	std::string animName;
	reader.readFile(initPath.c_str());
	std::vector<Luna::Vertex> vertices;
	std::vector<Vertex3D> vertices3D;
	reader.getVertices(0, vertices); //init to resize
	vertices3D.resize(vertices.size());
	mat = reader.getMaterial(0);
	reader.getJoints(joints);
	keyframePack.resize(joints.size());
	object->setSkeleton(joints, attackJoint);
	shared_ptr<Model> model = System::assetMananger->GetModel(initPath);
	System::assetMananger->LoadTexture(mat.diffuseTexPath, mat.diffuseTexPath);
	shared_ptr<Texture> texture = System::assetMananger->GetTexture(mat.diffuseTexPath);
	shared_ptr<Texture> glowmap;

	if (mat.hasGlowMap)
	{
		System::assetMananger->LoadGlowMap(mat.glowTexPath, mat.glowTexPath); //load texture
		glowmap = System::assetMananger->GetTexture(mat.glowTexPath); //set glow texture
	}
	mesh = reader.getMesh(0);
	if (model != nullptr)
	{
		model->SetTexture(texture);
		if (mat.hasGlowMap)
			model->setGlowMap(glowmap);
	}
	else if (model == nullptr)
	{
		reader.getVertices(0, vertices);
		for (int vrt = 0; vrt < vertices.size(); vrt++)
		{
			vertices3D[vrt] = vertices[vrt];
			vertices3D[vrt].uv.y = abs(1 - vertices3D[vrt].uv.y);
		}
		reader.getWeights(mesh.id, weights);
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
		model = shared_ptr<Model>(new Model());
		model->setMesh(vertices3D);
		model->SetTexture(texture); 	//set the texture to the model
		if (mat.hasGlowMap)
			model->setGlowMap(glowmap);

		System::assetMananger->LoadModel(filePath, model); //load model
		vertices3D.clear();
	}

	object->addModel(model, mesh.hasSkeleton);
	//set half size pushes in a array 
	//reader.readFile(initPath.c_str());
	if (mesh.hasBoundingBox)
		object->setHalfSize(reader.getBoundingBox(0).halfSize, reader.getBoundingBox(0).pos);

	for (int i = 0; i < animalAnimations.size(); i++) //nrOfAnimations
	{
		filePath = characterName + animalAnimations[i] + lu;
		reader.readFile(filePath.c_str());
		anims = reader.getAnimation();
		animName = std::string(anims.animationName);
		if (!object->checkIfAnimExist(animName, characterName))
		{
			for (int f = 0; f < joints.size(); f++)
				reader.getKeyframes(f, keyframePack[f]);
			object->setNewAnimation(anims.fps, anims.duration, animName, keyframePack);//change to pack
		}
		//set the texture to the model
	}
	
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
