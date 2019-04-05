#include "Particle.h"

Particle::Particle()
{
	//this->theVertex = { 0, 0, 0,
	//					0, 0, 
	//					0, 0, 0,
	//					0, 0, 0 };

	DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMMATRIX rotTemp = DirectX::XMMatrixRotationAxis(rotaxis, 0);
	DirectX::XMMATRIX scaleTemp = DirectX::XMMatrixScaling(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX translTemp = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&this->Rotation, rotTemp);
	DirectX::XMStoreFloat4x4(&this->Scale, scaleTemp);
	DirectX::XMStoreFloat4x4(&this->Translation, translTemp);
	this->position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);
}

Particle::~Particle()
{
}

void Particle::Shutdown()
{

}

void Particle::setPosition(DirectX::XMFLOAT3 position)
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	DirectX::XMMATRIX tempTransl = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMStoreFloat4x4(&this->Translation, tempTransl);
}

void Particle::setY(float y)
{
	this->position.y = y;
}

DirectX::XMFLOAT3 Particle::getPosition()
{
	return this->position;
}

Particle::ParticleType & Particle::getParticle()
{
	return this->properties;
}

//Vertex3D & Particle::getVertex()
//{
//	return this->theVertex;
//}

void Particle::setWorld()
{
	DirectX::XMMATRIX rotTemp;
	DirectX::XMMATRIX scaleTemp;
	DirectX::XMMATRIX translTemp;
	rotTemp = DirectX::XMLoadFloat4x4(&this->Rotation);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);
	DirectX::XMStoreFloat4x4(&this->world, (translTemp));
}

DirectX::XMFLOAT4X4 Particle::getWorld()
{
	return this->world;
}

void Particle::billboard(DirectX::XMFLOAT3 camPos)
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX translateMatrix;
	
	double angle = atan2(position.x- camPos.x, position.y-camPos.z) * (180.0 / DirectX::XM_PI);
	float rotation = (float)angle * 0.0174532925f;

	worldMatrix = DirectX::XMLoadFloat4x4(&world);
	worldMatrix = DirectX::XMMatrixRotationY(rotation);
	// Setup the translation matrix from the billboard model.
	translateMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translateMatrix);
	DirectX::XMStoreFloat4x4(&this->world, worldMatrix);
}
