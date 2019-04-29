#include "JointTransformation.h"

DirectX::XMFLOAT3 JointTransformation::interpolate(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float progression)
{
	DirectX::XMFLOAT3 interpol= { 0,0,0 };
	interpol.x = start.x + (end.x - start.x)*progression;
	interpol.y = start.y + (end.y - start.y)*progression;
	interpol.z = start.z + (end.z - start.z)*progression;
	return interpol;
}

JointTransformation::JointTransformation()
{
	this->position = {};
	this->rotation = {};
}

JointTransformation::JointTransformation(DirectX::XMFLOAT3 position, DirectX::XMVECTOR rotation)
{
	this->position = position;
	this->rotation = rotation;
}

JointTransformation::JointTransformation(float matrix[4][4])
{
	float x, y, z, w;
	float diagonal = matrix[0][0] + matrix[1][1] + matrix[2][2];
	if (diagonal > 0)
	{
		float w4 = (float)(sqrtf(diagonal + 1.f) * 2.f);
		w = w4 / 4.f;
		x = (matrix[2][1] - matrix[1][2]) / w4;
		y = (matrix[0][2] - matrix[2][0]) / w4;
		z = (matrix[1][0] - matrix[0][1]) / w4;
	}
	else if ((matrix[0][0]>matrix[1][1]) && (matrix[0][0] > matrix[2][2]))
	{
		float x4 = (float)(sqrtf(1.f+ matrix[0][0] - matrix[1][1] - matrix[2][2])*2.f);
		w= (matrix[2][1] - matrix[1][2]) / x4;
		x = x4 / 4.f;
		y = (matrix[1][0] + matrix[0][1]) / x4;
		z = (matrix[0][2] + matrix[2][0]) / x4;
	}
	else if (matrix[1][1] > matrix[2][2])
	{
		float y4 = (float)(sqrt(1.f + matrix[1][1] - matrix[0][0] - matrix[2][2])*2.f);
		w = (matrix[0][2] - matrix[2][0]) / y4;
		x = (matrix[1][0] + matrix[0][1]) / y4;
		y = y4 / 4.f;
		z = (matrix[2][1] + matrix[1][2]) / y4;
	}
	else
	{
		float z4 = (float)(sqrt(1.f + matrix[2][2] - matrix[0][0] - matrix[1][1])*2.f);
		w = (matrix[1][0] - matrix[0][1]) / z4;
		x = (matrix[0][2] + matrix[2][0]) / z4;
		y = z = (matrix[2][1] + matrix[1][2]) / z4;
		z = z4 / 4.f;
	}
	this->rotation.m128_f32[0] = x;
	this->rotation.m128_f32[1] = y;
	this->rotation.m128_f32[2] = z;
	this->rotation.m128_f32[3] = w;
	this->position.x = matrix[0][3];
	this->position.y = matrix[1][3];
	this->position.z = matrix[2][3];
	
}

JointTransformation::JointTransformation(const JointTransformation & obj)
{
	this->position = obj.position;
	this->rotation = obj.rotation;
}

JointTransformation::~JointTransformation()
{
}

void JointTransformation::operator=(const JointTransformation & obj)
{
	this->position = obj.position;
	this->rotation = obj.rotation;
}

DirectX::XMMATRIX JointTransformation::getLocalTransform()
{	
	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
	DirectX::XMMATRIX quaternionM = DirectX::XMMatrixRotationQuaternion(rotation);

	return DirectX::XMMatrixMultiply(matrix, quaternionM);
}

JointTransformation JointTransformation::interpolate(JointTransformation frameA, JointTransformation frameB, float progression) //progression is 0 to 1
{
	DirectX::XMFLOAT3 pos = this->interpolate(frameA.position, frameB.position, progression);
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionSlerp(frameA.rotation, frameB.rotation, progression);
	return JointTransformation(pos, quaternion);
}