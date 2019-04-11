#include "Transform.h"
const XMFLOAT3 & Transform::getPosition() const {
	return this->Position;
}

const XMFLOAT3 & Transform::getScale() const {
	return this->Scale;
}

const XMFLOAT3 & Transform::getRotation() const {
	return this->Rotation;
}

void Transform::move(float x, float y, float z)
{
	Position.x += x;
	Position.y += y;
	Position.z += z;
	ApplyTransform();
}

void Transform::setPosition(float x, float y, float z) {
	Position = XMFLOAT3(x, y, z);
	ApplyTransform();
}

void Transform::setScale(float x, float y, float z) {
	Scale = XMFLOAT3(x, y, z);
	ApplyTransform();
}

void Transform::setRotation(float x, float y, float z, float angle) {
	Angle = angle;
	this->Rotation = XMFLOAT3(x, y, z);
	ApplyTransform();

	//XMVECTOR te;
	//XMStoreFloat3(&test, te);
}

void Transform::ApplyTransform() {

	XMVECTOR Axis = XMVectorSet(Rotation.x, Rotation.y, Rotation.z, 1.0f);


	if (Angle != 0)
	{

		//if you want to rotate around a certain point
		this->world = DirectX::XMMatrixTranspose(
			XMMatrixScaling(Scale.x, Scale.y, Scale.z)*
			DirectX::XMMatrixRotationAxis(Axis, Angle)*
			XMMatrixTranslation(Position.x, Position.y, Position.z)
		);
		//	world = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationAxis(Axis, Angle));

	}
	else {
		this->world = DirectX::XMMatrixTranspose(
			XMMatrixScaling(Scale.x, Scale.y, Scale.z)*
			XMMatrixTranslation(Position.x, Position.y, Position.z));
	}
	world = DirectX::XMMatrixTranspose(world);
}
Transform::~Transform()
{
}
