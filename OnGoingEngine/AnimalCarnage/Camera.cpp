#include "Camera.h"

Camera::Camera()
{
	this->position = DirectX::XMFLOAT3(0.f, 1.f, -2.f);
	this->rotation = DirectX::XMFLOAT3(35.f, 0.f, 0.f);
	this->tempPosZ = this->position.z;
	this->accelerationX = 0;
	this->accelerationY = 0;
	this->accelerationZ = 0;
	this->tempPosX = this->position.x;
	this->tempPosY = this->position.y;
	this->tempPosZ = this->position.z;

	this->medianX = this->position.x;
	this->medianY = this->position.y;
	this->medianZ = this->position.z;
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

void Camera::cameraShake(float amount, DirectX::XMFLOAT3 direction)
{
	accelerationX += amount * direction.x;
	accelerationY += amount * direction.y;
	accelerationZ += amount * direction.z;
}

DirectX::XMFLOAT3 Camera::GetPosition()
{
	return this->position;
}

DirectX::XMFLOAT3 Camera::GetRotation()
{
	return this->rotation;
}

DirectX::XMVECTOR Camera::GetUp() const
{
	return this->Up;
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
	//float min = 10000.f;
	//float max = 0.f; 
	//float medX = 0.f;
	//float medY = 0.f;

	//for (int i = 0; i < playerPos.size(); i++)
	//{
	//	min = fminf(playerPos[i].x, min);
	//	max = fmaxf(playerPos[i].x, max);
	//	medX += playerPos[i].x;
	//	medY += playerPos[i].y;
	//}

	//medX /= playerPos.size();
	//medY /= playerPos.size();

	//float length = max - min;
	//if (length < 7.f)
	//	length = 7.f;
	//else if (length > 20.f)
	//	length = 20.f;
	//this->position = DirectX::XMFLOAT3(medX, medY, -length);


	float min = 10000.f;
	float max = -10000000000.f; 

	float minY = 10000.f;
	float maxY = -10000000000.f;

	float medX = 0.f;
	float medY = 0.f;

	for (int i = 0; i < playerPos.size(); i++)
	{
		min = fminf(playerPos[i].x, min);
		max = fmaxf(playerPos[i].x, max);

		minY = fminf(playerPos[i].y, minY);
		maxY = fmaxf(playerPos[i].y, maxY);

		medX += playerPos[i].x;
		medY += playerPos[i].y;
	}

	medX /= playerPos.size();
	medY /= playerPos.size();



	float length = 0;

	if (min < max)
		length = (max - min);
	if (min < max)
		length = (min - max);

	float lengthY = 0;

	if (minY < maxY)
		lengthY = (maxY - minY);
	if (minY < maxY)
		lengthY = (minY - maxY);

	lengthY *= 1.2;

	//Zoom compensation
	//medY = (medY - length/4);

	if (length > lengthY) {
		length = lengthY;
	}
	length *= 0.35;
	length -= 5;


	if (length > 20) {
		length = 20;
	}
	if (length < -20) {
		length = -20;
	}

	medY = minY;
	//Zoom compensation
	medX = medX / fmaxf(1, -length/10);
	medY = (medY - length/1.2) + (length *0.3) +3.5; 

	float distanceX;
	float distanceY;

	//if (this->position.x < medX) {
	//	distanceX = this->position.x - medX;
	//	acceleration.x -= 0.01;
	//	//this->position.x -= distanceX / 20);
	//}
	//if (this->position.x > medX) {
	//	distanceX = medX - this->position.x;
	//	acceleration.x += 0.01;
	//	//this->position.x += distanceX / 20);
	//}

	//if (this->position.y < medY) {
	//	distanceY = this->position.y - medY;
	//	acceleration.y -= 0.01;
	//	//this->position.y -= distanceY / 20);
	//}
	//if (this->position.y > medY) {
	//	distanceY = medY - this->position.y;
	//	acceleration.y += 0.01;
	//	//this->position.y += distanceY / 20);
	//}

	if (this->position.x < medX) {
		distanceX = this->position.x - medX;
		this->accelerationX += 0.0005;
		//this->position.x -= distanceX / 20);
		tempPosX -= distanceX / 12.5;
	}
	if (this->position.x > medX) {
		distanceX = medX - this->position.x;
		this->accelerationX -= 0.0005;
		//this->position.x += distanceX / 20);
		tempPosX += distanceX / 12.5;
	}

	if (this->position.y < medY) {
		distanceY = this->position.y - medY;
		this->accelerationY += 0.0005;
		//this->position.y -= distanceY / 20);
		tempPosY -= distanceY / 12.5;
	}
	if (this->position.y > medY) {
		distanceY = medY - this->position.y;
		this->accelerationY -= 0.0005;
		//this->position.y += distanceY / 20);
		tempPosY += distanceY / 12.5;
	}

	//Re-Center when zoomed out
	/*if (position.y > 0) {

	}
	if(){}*/

	float distanceZ;

	if (length < this->position.z) {
		distanceZ = (this->position.z - length);
		this->tempPosZ -= distanceZ / 12.5;
		this->tempPosZ = this->tempPosZ - 0.65;
		accelerationZ -= 0.0005;
	}
	else if(length > this->position.z) {
		distanceZ = (length - this->position.z);
		this->tempPosZ += distanceZ / 12.5;
		this->tempPosZ = this->tempPosZ - 0.65;
		accelerationZ += 0.0005;
	}

	this->accelerationX /= 1.05;
	this->accelerationY /= 1.05;
	this->accelerationZ /= 1.05;

	medianX = tempPosX;
	medianY = tempPosY;
	medianZ = tempPosZ;
	//medianX += this->accelerationX;
	//medianY += this->accelerationY;
	//medianZ += this->accelerationZ;
	/*medianX /= 2;
	medianY /= 2;
	medianZ /= 2;*/
	

	//New funny camera
	this->position.x += this->accelerationX;
	this->position.y += this->accelerationY;
	this->position.z += this->accelerationZ;
	this->position.x = (this->position.x*15 + medianX)/16;
	this->position.y = (this->position.y*15 + medianY)/16;
	this->position.z = (this->position.z*15 + medianZ)/16;


	//Daniels stable camera
	/*this->position.x = medianX;
	this->position.y = medianY;
	this->position.z = medianZ;*/

	//this->position.z = tempPosZ;

	/*if (length < this->position.z) {
		this->position.z += length /20;
	}
	else if (length > this->position.z) {
		this->position.z -= length / 20;
	}*/

	//this->position.z = -length;
}
