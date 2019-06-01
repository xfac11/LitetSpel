

#include "Transform.h"
const XMFLOAT3& Transform::getPosition() const {
	return this->Position;
}

const DirectX::SimpleMath::Vector3 & Transform::getPosition()
{
 return	DirectX::SimpleMath::Vector3(Position.x,Position.y,Position.z);
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

void Transform::setPosition(btVector3 xyz)
{
	Position = XMFLOAT3(xyz.getX(), xyz.getY(), xyz.getZ());
	ApplyTransform();
}

void Transform::setPosition(XMFLOAT3 xyz) {
	Position = xyz;
	ApplyTransform();
}

void Transform::setPosition(float x, float y, float z) {
	Position = XMFLOAT3(x, y, z);
	ApplyTransform();
}

void Transform::setScale(btVector3 xyz)
{
	Scale = XMFLOAT3(xyz.getX(), xyz.getY(), xyz.getZ());
	ApplyTransform();
}

void Transform::setScale(XMFLOAT3 xyz)
{
	Scale = xyz;
	ApplyTransform();
}

void Transform::setScale(float x, float y, float z) {
	Scale = XMFLOAT3(x, y, z);
	ApplyTransform();
}

void Transform::setRotation(float x, float y, float z, float angle) {
	Angle = angle;
	this->Rotation = XMFLOAT3(x, y, z);
	this->rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(x, y, z);

	ApplyTransform();
	//XMVECTOR te;
	//XMStoreFloat3(&test, te);
}

void Transform::setRotationRollPitchYaw(float x, float y, float z)
{
	this->Rotation = XMFLOAT3(x, y, z);
	Angle = 1;
	//Angle = angle;
	//this->Rotation = XMFLOAT3(Rotation.x, Rotation.y, angle);
	this->rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(x,y,z);
	ApplyTransform();
}

void Transform::setMatrix(DirectX::XMMATRIX theMatrix)
{
	//DirectX::XMMATRIX temp = DirectX::XMMatrixTranslation(this->Position.x, this->Position.y, this->Position.z);
	this->world = theMatrix; //this->world*


	DirectX::XMVECTOR tempQuaternion;
	DirectX::XMVECTOR tempPosition;
	DirectX::XMVECTOR tempScale;
	

	DirectX::XMMatrixDecompose(&tempScale, &tempQuaternion, &tempPosition, DirectX::XMMatrixTranspose(world));

	DirectX::XMStoreFloat3(&this->Position, tempPosition);
	this->Rotation.x = getRoll(tempQuaternion);
	this->Rotation.y = getPitch(tempQuaternion);
	this->Rotation.z = getYaw(tempQuaternion);
	DirectX::XMStoreFloat3(&this->Scale, tempScale);
	ApplyTransform();
}

float Transform::getPitch(DirectX::XMVECTOR Quaternion)
{
	return atan2(2 * (Quaternion.m128_f32[1] * Quaternion.m128_f32[2] + Quaternion.m128_f32[3] * Quaternion.m128_f32[0]), Quaternion.m128_f32[3] * Quaternion.m128_f32[3] - Quaternion.m128_f32[0] * Quaternion.m128_f32[0] - Quaternion.m128_f32[1] * Quaternion.m128_f32[1] + Quaternion.m128_f32[2] * Quaternion.m128_f32[2]);
}

float Transform::getYaw(DirectX::XMVECTOR Quaternion)
{
	return asin(-2 * (Quaternion.m128_f32[0] * Quaternion.m128_f32[2] - Quaternion.m128_f32[3] * Quaternion.m128_f32[1]));
}

float Transform::getRoll(DirectX::XMVECTOR Quaternion)
{
	return atan2(2 * (Quaternion.m128_f32[0] * Quaternion.m128_f32[1] + Quaternion.m128_f32[3] * Quaternion.m128_f32[2]), Quaternion.m128_f32[3] * Quaternion.m128_f32[3] + Quaternion.m128_f32[0] * Quaternion.m128_f32[0] - Quaternion.m128_f32[1] * Quaternion.m128_f32[1] - Quaternion.m128_f32[2] * Quaternion.m128_f32[2]);
}


void Transform::ApplyTransform() {

	XMVECTOR Axis = XMVectorSet(Rotation.x, Rotation.y, Rotation.z, 1.0f);


	if (Angle != 0)
	{

		//if you want to rotate around a certain point
		this->world = 
			this->rotationMatrix*
			XMMatrixScaling(Scale.x, Scale.y, Scale.z) *
			//*DirectX::XMMatrixRotationAxis(Axis, Angle) **/
			XMMatrixTranslation(Position.x, Position.y, Position.z);
		//	world = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationAxis(Axis, Angle));

	}
	else {
		this->world = 
			XMMatrixScaling(Scale.x, Scale.y, Scale.z) *
			XMMatrixTranslation(Position.x, Position.y, Position.z);
	}
	world = DirectX::XMMatrixTranspose(world);
}
Transform::~Transform()
{

}

void Transform::SetWorldToRigitBody()
{
	 this->world = btTransform_to_XMMATRIX(this->rigidbody->getWorldTransform());
}

void Transform::SetPosition(float x, float y, float z)
{
	btTransform t = this->rigidbody->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	rigidbody->setWorldTransform(t);
}

XMFLOAT3 Transform::GetPosition()
{
	btTransform t =  this->rigidbody->getWorldTransform();
	return XMFLOAT3(t.getOrigin().getX(), t.getOrigin().getY(),t.getOrigin().getZ());
}

void Transform::Move(XMFLOAT3 posion)
{
	btTransform t = this->rigidbody->getWorldTransform();
	btVector3 pos = t.getOrigin();
	t.setOrigin(btVector3(pos.getX() + posion.x, pos.getY()+ posion.y, pos.getZ() + posion.z));
	rigidbody->setWorldTransform(t);
}

XMFLOAT3 normalize(XMFLOAT3 pos)
{
	float len = magnitude(pos);
	if (len != 0.)
		return XMFLOAT3(pos.x /= len, pos.y /= len, pos.z /= len);
	else
		return pos;
}

float magnitude(XMFLOAT3 pos)
{
	return sqrtf( (pos.x * pos.x) +( pos.y * pos.y )+ (pos.z * pos.z));
}

XMFLOAT3 VECTORSUBTRACTION(const XMFLOAT3 other, const XMFLOAT3 other2)
{
	return XMFLOAT3(other.x - other2.x, other.y - other2.y, other.z - other2.z);
}

XMFLOAT3 MULT(const XMFLOAT3 other, const XMFLOAT3 other2)
{
	return XMFLOAT3(other.x* other2.x, other.y* other2.y, other.z* other2.z);
}

XMFLOAT3 MULT(const XMFLOAT3 other, const float other2)
{
	return XMFLOAT3(other.x * other2, other.y*other2, other.z * other2);
}


btTransform XMMATRIX_to_btTransform(XMMATRIX const& mat)
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

XMMATRIX btTransform_to_XMMATRIX(btTransform const& trans)
{
	//store btTranform in 4x4 Matrix
	XMFLOAT4X4 matrix4x4 = XMFLOAT4X4();
	btMatrix3x3 const& Rotation = trans.getBasis();
	btVector3 const& Position = trans.getOrigin();
	// copy rotation matrix
	for (int row = 0; row < 3; ++row)
	{

		for (int column = 0; column < 3; ++column)
		{
			matrix4x4.m[row][column] = Rotation[column][row];
		}
	}

	// copy position
	for (int column = 0; column < 3; ++column)
	{
		matrix4x4.m[3][column] = Position[column];
	}
	return XMLoadFloat4x4(&matrix4x4);
}

XMFLOAT3 btTransform_to_XMFLOAT3(btTransform const& trans)
{
	btVector3 const Position = trans.getOrigin();

	return XMFLOAT3(Position.getX(),Position.getY(),Position.getZ());
}

