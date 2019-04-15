#include "GameObject.h"

GameObject::GameObject()
{
	this->cap=5;
	this->nrOfModels = 0;
	this->theModel = new Model[this->cap];
	this->theTransforms = Transform();
}

GameObject::GameObject(Shader * shader)
{
	this->cap = 5;
	this->nrOfModels = 0;
	this->theModel = new Model[this->cap];
	this->theTransforms = Transform();
	this->theModel[0].setShader(shader, Opaque);
	//this->worldConstBuffer.initialize();
}

GameObject::~GameObject()
{
	delete[] this->theModel;
}

void GameObject::setPosition(float x, float y, float z)
{
	this->theTransforms.setPosition(x, y, z);
}

void GameObject::setRotation()
{
	//this->theTransforms.setRotation();
}

void GameObject::setScale(float x, float y, float z)
{
	this->theTransforms.setScale(x, y, z);
}

void GameObject::move(float x, float y, float z)
{
	this->theTransforms.move(x, y, z);
}

DirectX::XMFLOAT3 GameObject::getPosition()
{
	return this->theTransforms.getPosition();
}

int GameObject::getNrOfModels()
{
	return this->nrOfModels;
}

Model & GameObject::getModel(int id)
{
	return this->theModel[id];
}

void GameObject::setMesh(std::vector<Vertex3D> mesh, DWORD * indices, int numberOfIndices, int id)
{
	this->theModel[id].setMesh(mesh, indices, numberOfIndices);
}

void GameObject::setTexture(std::string file, int id)
{									
	this->theModel[id].setTexture(file);
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

	this->theModel[0].getShader()->setWorld(this->theTransforms.getWorld());
	this->theModel[0].draw();
}
