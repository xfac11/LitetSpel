

#include "Transform.h"
const XMFLOAT3& Transform::getPosition() const {
	return this->Position;
}

const XMFLOAT3& Transform::getScale() const {
	return this->Scale;
}

const XMFLOAT3& Transform::getRotation() const {
	return this->Rotation;
}

void Transform::move(XMFLOAT3 p)
{
	Position.x += p.x;
	Position.y += p.y;
	Position.z += p.z;
	ApplyTransform();
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
			XMMatrixScaling(Scale.x, Scale.y, Scale.z) *
			DirectX::XMMatrixRotationAxis(Axis, Angle) *
			XMMatrixTranslation(Position.x, Position.y, Position.z)
		);
		//	world = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationAxis(Axis, Angle));

	}
	else {
		this->world = DirectX::XMMatrixTranspose(
			XMMatrixScaling(Scale.x, Scale.y, Scale.z) *
			XMMatrixTranslation(Position.x, Position.y, Position.z));
	}
	world = DirectX::XMMatrixTranspose(world);
}
Transform::~Transform()
{

}

btTransform Transform::XMMATRIX_to_btTransform(XMMATRIX const& mat)
{
	// convert from XMMATRIX to btTransform (Bullet Physics)
	btMatrix3x3 Rotation;
	btVector3 Position;
	// copy rotation matrix

	XMFLOAT4X4 xmatrix;
	XMStoreFloat4x4(&xmatrix, mat);

	for (int row = 0; row < 3; ++row)

		for (int column = 0; column < 3; ++column)

			Rotation[row][column] = xmatrix.m[column][row];

	// copy position
	for (int column = 0; column < 3; ++column)

		Position[column] = xmatrix.m[3][column];

	return btTransform(Rotation, Position);
}

XMMATRIX Transform::btTransform_to_XMMATRIX(btTransform const& trans)
{
	//store btTranform in 4x4 Matrix
	XMFLOAT4X4 matrix4x4;
	btMatrix3x3 const& Rotation = trans.getBasis();
	btVector3 const& Position = trans.getOrigin();
	// copy rotation matrix
	for (int row = 0; row < 3; ++row)

		for (int column = 0; column < 3; ++column)

			matrix4x4.m[row][column] = Rotation[column][row];

	// copy position
	for (int column = 0; column < 3; ++column)

		matrix4x4.m[3][column] = Position[column];

	return XMLoadFloat4x4(&matrix4x4);
}

XMFLOAT3 Transform::btTransform_to_XMFLOAT3(btTransform const& trans)
{
	btVector3 const Position = trans.getOrigin();

	return XMFLOAT3(Position.getX(),Position.getY(),Position.getZ());
}

