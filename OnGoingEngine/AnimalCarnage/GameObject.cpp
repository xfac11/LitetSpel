#include "GameObject.h"

GameObject::GameObject()
{
	this->theModel = new Model();
	this->theTransforms = Transform();
}

GameObject::GameObject(Shader * shader)
{
	this->theModel = new Model();
	this->theTransforms = Transform();
	this->theModel->setShader(shader);
	//this->worldConstBuffer.initialize();
}

GameObject::~GameObject()
{
	delete this->theModel;
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

DirectX::XMFLOAT3 GameObject::getScale()
{
	return this->theTransforms.getScale();
}

void GameObject::setMesh(std::vector<Vertex3D> mesh, DWORD * indices, int numberOfIndices)
{
	this->theModel->setMesh(mesh, indices, numberOfIndices);
}

void GameObject::setTexture(std::string file)
{
	this->theModel->setTexture(file);
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

	this->theModel->getShader()->setWorld(this->theTransforms.getWorld());
	this->theModel->draw();
}
