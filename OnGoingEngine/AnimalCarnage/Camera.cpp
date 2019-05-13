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

void Camera::move(DirectX::XMFLOAT3 position)
{
	//z forward
	//x left right

	this->position.y += position.y;
	//forward backward
	if (abs(this->rotation.y) <= 90)
	{
		this->position.z += (90 - abs(this->rotation.y))*position.z;
		this->position.x += (this->rotation.y)*position.z;
	}
	else if (abs(this->rotation.y >= 90))
	{
		this->position.z += (abs(this->rotation.y) - 90)*position.z;
		this->position.x += (180 + (this->rotation.y))*position.z;
	}

	//left right
	if (abs(this->rotation.y) <= 90)
	{
		this->position.z += (-this->rotation.y)*position.x;
		this->position.x += (90 - abs(this->rotation.y))*position.x;
	}
	else if (abs(this->rotation.y >= 90))
	{
		this->position.z += (180+(this->rotation.y))*position.x;
		this->position.x += (abs(this->rotation.y)-90)*position.x;
	}
	
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

void Camera::rotate(DirectX::XMFLOAT3 rotation)
{
	if (abs(this->rotation.x) > 87.f)
		rotation.x = -rotation.x;
	if (abs(this->rotation.y + rotation.y) >= 90)
		rotation.y = 0;

	this->rotation.x += rotation.x;
	this->rotation.y += rotation.y;
	this->rotation.z += rotation.z;
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
	float dir[4] = {
		0.0f,-1,0.0f,1.0f
	};
	DirectX::XMVECTOR lightDirView = DirectX::XMVectorSet(0,0,0, 0);
	DirectX::XMVECTOR camPos = DirectX::XMVectorSet(6.0f, 5.0f, 0.0f, 0);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);
	//DirectX::XMMatrixLookAtLH(CamPos, lightDirView, up);
	//this->viewMatrix = DirectX::XMMatrixLookAtLH(camPos, lightDirView, up);
	this->viewMatrix = DirectX::XMMatrixLookAtLH(CamPos, LookAt, Up);
	//this->viewMatrix = DirectX::XMMatrixLookAtRH(CamPos, LookAt, Up);
	//this->viewMatrix = DirectX::XMMatrixTranspose(viewMatrix); //moved to shader
}

DirectX::XMMATRIX& Camera::GetViewMatrix()
{
	return this->viewMatrix;
}

void Camera::calcCamera(std::vector<DirectX::XMFLOAT3> playerPos)
{
	float min = 10000.f;
	float max = 0.f; 
	float medX = 0.f;
	float medY = 0.f;

	for (int i = 0; i < playerPos.size(); i++)
	{
		min = fminf(playerPos[i].x, min);
		max = fmaxf(playerPos[i].x, max);
		medX += playerPos[i].x;
		medY += playerPos[i].y;
	}

	medX /= playerPos.size();
	medY /= playerPos.size();

	float length = max - min;
	if (length < 7.f)
		length = 7.f;
	else if (length > 20.f)
		length = 20.f;
	this->position = DirectX::XMFLOAT3(medX, medY, -length);
}
