#include "Camera.h"

Camera::Camera()
{
	this->position = DirectX::XMFLOAT3(0.f, 1.f, -2.f);
	this->rotation = DirectX::XMFLOAT3(35.f, 0.f, 0.f);

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

void Camera::move(float x, float y, float z)
{
	this->position = DirectX::XMFLOAT3(this->position.x + x, this->position.y + y, this->position.z + z);
}

void Camera::rotate(float x, float y, float z)
{
	if (this->rotation.x + x >= 90)
	{
		DirectX::XMFLOAT3(90, this->rotation.y + y, this->rotation.z + z);
	}
	else
	{
		this->rotation = DirectX::XMFLOAT3(this->rotation.x + x, this->rotation.y + y, this->rotation.z + z);
	}
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

void Camera::calcCamera(DirectX::XMFLOAT3 playerOne, DirectX::XMFLOAT3 playerTwo, DirectX::XMFLOAT3 playerThree, DirectX::XMFLOAT3 playerFour)
{
	float min = fminf(playerOne.x, fminf(playerTwo.x, fminf(playerThree.x, playerFour.x)));
	float max = fmaxf(playerOne.x,fmaxf(playerTwo.x, fmaxf(playerThree.x, playerFour.x)));

	float length = max - min;
	if (length < 2.5f)
	{
		length = 2.5f;
	}
	this->position = DirectX::XMFLOAT3(0, 0, -length);
}
