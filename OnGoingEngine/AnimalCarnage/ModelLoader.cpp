#include "ModelLoader.h"
#include "System.h"

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
		//object[i] = new GameObject;
	

		//reader.getMaterial(i).

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
				//keyframePack[i].resize()
				reader.getKeyframes(i, keyframePack[i]);
			}
			//reader.getKeyframes(15, keyframes);

			
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

			//int vertexId = 0;
			//for (int p = 0; p < weights.size() / 3; p++)
			//{
			//	for (int v = 0; v < 3; v++)
			//	{
			//		//int ctrlPointIdx = -1;
			//		//if (p >= 0 && p < weights.size() / 3 && v >= 0 && v < 3)
			//		//{
			//		//	//weigh[vertexId];
			//		//}

			//		//float sumWeights = weights[vertexId].weights[0] + weights[vertexId].weights[1] + weights[vertexId].weights[2] + weights[vertexId].weights[3];
			//		vertices3D[vertexId].Joint.x = weights[vertexId].jointIDs[0];
			//		vertices3D[vertexId].Joint.y = weights[vertexId].jointIDs[1];
			//		vertices3D[vertexId].Joint.z = weights[vertexId].jointIDs[2];
			//		vertices3D[vertexId].Joint.w = weights[vertexId].jointIDs[3];

			//		vertices3D[vertexId].Weights.x = weights[vertexId].weights[0];
			//		vertices3D[vertexId].Weights.y = weights[vertexId].weights[1];
			//		vertices3D[vertexId].Weights.z = weights[vertexId].weights[2];
			//		vertices3D[vertexId].Weights.w = weights[vertexId].weights[3];
			//		vertexId++;
			//	}
			//}
			

		}
		else
		{
		/*		
			std::vector<Luna::Keyframe> keyframes;
			Luna::Keyframe temp;
			keyframes.push_back(temp);
			object[i].setKeyFrameData(keyframes, false);
		*/
		}

		if(mesh.hasBoundingBox)
			object[i].setHalfSize(reader.getBoundingBox(i).halfSize, reader.getBoundingBox(i).pos);
		object[i].addModel(vertices3D, dIndices, (int)indices.size(), mesh.hasSkeleton); //mesh.hasSkeleton
		object[i].setTexture(reader.getMaterial(i).diffuseTexPath,i);
		
		if (mat.hasGlowMap)
			object->setGlowMap(mat.glowTexPath, i);

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
