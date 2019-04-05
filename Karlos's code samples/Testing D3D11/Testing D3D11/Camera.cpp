#include "Camera.h"

Camera::Camera()
{
	this->position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	this->rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	this->position = DirectX::XMFLOAT3(x, y, z);
}

void Camera::SetPosition(DirectX::XMFLOAT3 position)
{
	this->position = position;
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rotation = DirectX::XMFLOAT3(x, y, z);
}

void Camera::SetRotation(DirectX::XMFLOAT3 rotation)
{
	this->rotation = rotation;
}

DirectX::XMFLOAT3 Camera::GetPosition()
{
	return this->position;
}

DirectX::XMFLOAT3 Camera::GetRotation()
{
	return this->rotation;
}

void Camera::Render()
{
	float radians = 0.0174532925f;

	

	DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.f, 0.0f, 0.1f, 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0, 1.f, 0, 0.0);
	DirectX::XMVECTOR CamPos = DirectX::XMVectorSet(this->position.x, this->position.y, this->position.z, 0.0);
	DirectX::XMFLOAT3 YawPitchRoll = DirectX::XMFLOAT3(this->rotation.x*radians, this->rotation.y*radians, this->rotation.z*radians);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(YawPitchRoll.x, YawPitchRoll.y, YawPitchRoll.z);
	
	DirectX::XMVECTOR lookOffset = DirectX::XMVector4Transform(Forward, rotationMatrix);
	//lookOffset = DirectX::XMVectorAdd(CamPos, Forward);
	DirectX::XMVECTOR LookAt = DirectX::XMVectorAdd(CamPos, lookOffset);
	//LookAt = DirectX::XMVector3TransformCoord(LookAt, rotationMatrix);
	//Up = XMVector3TransformCoord(Up, rotationMatrix);
	//LookAt = DirectX::XMVectorAdd(CamPos, LookAt);
	
	this->viewMatrix = DirectX::XMMatrixLookAtLH(CamPos, LookAt, Up);
	//this->viewMatrix = DirectX::XMMatrixLookAtRH(CamPos, LookAt, Up);
	//this->viewMatrix = DirectX::XMMatrixTranspose(viewMatrix); //moved to shader
}

DirectX::XMMATRIX& Camera::GetViewMatrix()
{
	return this->viewMatrix;
}