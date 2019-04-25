#include "GameObject.h"

GameObject::GameObject()
{
	this->cap=5;
	this->nrOfModels = 0;
	this->theModel = new Model*[this->cap];
	for (int i = 0; i < cap; i++)
	{
		this->theModel[i] = nullptr;
	}
	this->theTransforms = Transform();
}

GameObject::GameObject(Shader * shader)
{
	this->cap = 5;
	this->nrOfModels = 0;
	this->theModel = new Model*[this->cap];
	for (int i = 0; i < cap; i++)
	{
		this->theModel[i] = nullptr;
	}
	this->theTransforms = Transform();
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

void GameObject::addModel(std::vector<Vertex3D> mesh, DWORD * indices, int numberOfIndices)
{
	this->theModel[nrOfModels] = new Model;
	this->theModel[nrOfModels]->setMesh(mesh, indices, numberOfIndices);
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
