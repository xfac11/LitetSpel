#include "GameObject.h"
#include "System.h"

GameObject::GameObject()
{
	this->hasLoadedAABB = false;
	this->cap=5;
	this->nrOfModels = 0;
	this->theModel = new Model*[this->cap];
	for (int i = 0; i < cap; i++)
	{
		this->theModel[i] = nullptr;
	}
	this->colBox = AABB();
}

GameObject::GameObject(Shader * shader)
{
	this->hasLoadedAABB = false;
	this->cap = 5;
	this->nrOfModels = 0;
	this->theModel = new Model*[this->cap];
	for (int i = 0; i < cap; i++)
	{
		this->theModel[i] = nullptr;
	}
	this->colBox = AABB();

	
	/*this->theModel[0] = new Model;
	this->theModel[0]->setShader(shader);
	this->nrOfModels++;*/

	//this->worldConstBuffer.initialize();
}


GameObject::~GameObject()
{
	for (int i = 0; i < this->nrOfModels; i++)
	{
		delete this->theModel[i];
	}
	delete[] this->theModel;
}


int GameObject::getNrOfModels()
{
	return this->nrOfModels;
}

Model *& GameObject::getModel(int id)
{
	return this->theModel[id];
}

Model **& GameObject::getTheModelPtr()
{
	return this->theModel;
}

void GameObject::setHalfSize(float halfSize[3], float posOffset[3])
{
	
	this->hasLoadedAABB = true;
	this->colBox.width = halfSize[0];
	this->colBox.height = halfSize[1];
	this->colBox.depth = halfSize[2];
	//wthis->colBox.positionOffset = btVector3(posOffset[0], posOffset[1], posOffset[2]);
	this->CollisionShape = new Primitives();
	this->CollisionShape->Initialize(1,btVector3(posOffset[0], posOffset[1], posOffset[2]), btVector3(halfSize[0]*2, halfSize[1]*2, halfSize[2]*2));
	positionOffset = btVector3(posOffset[0], posOffset[1], posOffset[2]);
	
	//DirectX::XMMatrixTranslation(this->Position.x+posOffset[0])
	this->CollisionShape->SetWorld(&this->getWorld());
	System::getDebugDraw()->addPrimitives(this->CollisionShape);
}

void GameObject::calcAABB(std::vector<Vertex3D> mesh)
{
	if (!this->hasLoadedAABB)
	{
		int minX = 0;
		int minY = 0;
		int minZ = 0;
		int maxX = 0;
		int maxY = 0;
		int maxZ = 0;
		for (int i = 1; i < mesh.size(); i++)
		{
			if (mesh.at(i).position.x < mesh.at(minX).position.x)
			{
				minX = i;
			}
			if (mesh.at(i).position.y < mesh.at(minY).position.y)
			{
				minY = i;
			}
			if (mesh.at(i).position.z < mesh.at(minZ).position.z)
			{
				minZ = i;
			}

			if (mesh.at(i).position.x > mesh.at(maxX).position.x)
			{
				maxX = i;
			}
			if (mesh.at(i).position.y > mesh.at(maxY).position.y)
			{
				maxY = i;
			}
			if (mesh.at(i).position.z > mesh.at(maxZ).position.z)
			{
				maxZ = i;
			}
		}


		DirectX::XMFLOAT3 max;
		DirectX::XMFLOAT3 min;
		max = DirectX::XMFLOAT3(mesh.at(maxX).position.x, mesh.at(maxY).position.y, mesh.at(maxZ).position.z);
		min = DirectX::XMFLOAT3(mesh.at(minX).position.x, mesh.at(minY).position.y, mesh.at(minZ).position.z);
		/*this->colBox.Max.x = max.x * this->Scale.x;
		this->colBox.Max.y = max.y * this->Scale.y;
		this->colBox.Max.z = max.z * this->Scale.z;
		this->colBox.Min.x = min.x * this->Scale.x;
		this->colBox.Min.y = min.y * this->Scale.y;
		this->colBox.Min.z = min.z * this->Scale.z;*/


		this->colBox.width = max.x;
		this->colBox.height = max.y;
		this->colBox.depth = max.z;
	}
	
	/*this->colBox.Min = min;
	this->colBox.Max = max;*/
}

void GameObject::addModel(std::vector<Vertex3D> mesh, DWORD * indices, int numberOfIndices)
{
	this->theModel[nrOfModels] = new Model;
	this->theModel[nrOfModels]->setMesh(mesh, indices, numberOfIndices);
	//if(this->colBox.Max.x == 1.0f)
		this->calcAABB(mesh);
	//this->theModel[nrOfModels]->setSampler();
	nrOfModels++;
}

void GameObject::setMesh(std::vector<Vertex3D> mesh, DWORD * indices, int numberOfIndices, int id)
{
	this->theModel[id]->setMesh(mesh, indices, numberOfIndices);
}

void GameObject::setTexture(std::string file, int id)
{									
	this->theModel[id]->setTexture(file);
}

void GameObject::setGlowMap(std::string file, int id)
{
	this->theModel[id]->setGlowMap(file);
}

void GameObject::draw()
{
	/*ForwardShader* ptr = nullptr;
	ptr = dynamic_cast<ForwardShader*>(this->theModel->getShader());
	if (ptr != nullptr)
	{
		ptr->setWorld(this->theTransforms.getWorld());
		this->theModel->draw();
	}*/
	/*for (int i = 0; i < this->nrOfModels; i++)
	{
		this->theModel[0]->getShader()->setWorld(this->theTransforms.getWorld());
		this->theModel[0]->draw();
	}*/


	this->theModel[0]->getShader()->setWorld(this->getWorld());
	this->theModel[0]->draw();
}

AABB GameObject::getCollisionBox()
{
	AABB tempBox;
	//updates the collision box
	/*tempBox.Max.x = tempBox.Max.x * this->Scale.x;
	tempBox.Max.y = tempBox.Max.y * this->Scale.y;
	tempBox.Max.z = tempBox.Max.z * this->Scale.z;

	tempBox.Min.x = tempBox.Min.x * this->Scale.x;
	tempBox.Min.y = tempBox.Min.y * this->Scale.y;
	tempBox.Min.z = tempBox.Min.z * this->Scale.z;

	tempBox.Max.x = this->colBox.Max.x + this->Position.x;
	tempBox.Max.y = this->colBox.Max.y + this->Position.y;
	tempBox.Max.z = this->colBox.Max.z + this->Position.z;

	tempBox.Min.x = this->colBox.Min.x + this->Position.x;
	tempBox.Min.y = this->colBox.Min.y + this->Position.y;
	tempBox.Min.z = this->colBox.Min.z + this->Position.z;
	*/  

	/*tempBox.Max.x = this->colBox.Max.x + this->Position.x;
	tempBox.Max.y = this->colBox.Max.y + this->Position.y;
	tempBox.Max.z = this->colBox.Max.z + this->Position.z;

	tempBox.Min.x = this->colBox.Min.x + this->Position.x;
	tempBox.Min.y = this->colBox.Min.y + this->Position.y;
	tempBox.Min.z = this->colBox.Min.z + this->Position.z;*/

	tempBox.width = this->colBox.width*this->Scale.x;
	tempBox.height = this->colBox.height*this->Scale.y;
	tempBox.depth = this->colBox.depth*this->Scale.z;
	return tempBox;
}
