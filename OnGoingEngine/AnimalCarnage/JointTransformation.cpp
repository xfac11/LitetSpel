#include "JointTransformation.h"

//DirectX::XMFLOAT3 JointTransformation::interpolate(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float progression)
//{
//	DirectX::XMFLOAT3 interpol= { 0,0,0 };
//	interpol.x = start.x + (end.x - start.x)*progression;
//	interpol.y = start.y + (end.y - start.y)*progression;
//	interpol.z = start.z + (end.z - start.z)*progression;
//	return interpol;
//}
//
//DirectX::XMFLOAT3 JointTransformation::interpolate2(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float progression)
//{
//	DirectX::XMFLOAT3 interpol = { 0,0,0 };
//	interpol.x = start.x * (1 - progression) + end.x * progression;
//	interpol.y = start.y * (1 - progression) + end.y * progression;
//	interpol.z = start.z * (1 - progression) + end.z * progression;
//	return interpol;
//}


JointTransformation::JointTransformation()
{
	this->position = {};
	this->rotation = {};
	this->scale = {};
}

JointTransformation::JointTransformation(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 rotation, DirectX::XMFLOAT4 scale)
{
	this->position = DirectX::XMFLOAT3(position.x,position.y, position.z);
	this->rotation = DirectX::XMLoadFloat4(&rotation);
	
	this->scale = DirectX::XMFLOAT3(scale.x, scale.y, scale.z);
}

JointTransformation::JointTransformation(DirectX::XMFLOAT4 position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 scale)
{
	this->position = DirectX::XMFLOAT3(position.x, position.y, position.z);
	this->rotation = rotation;
	this->scale = scale;
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
	this->scale.x = matrix[0][0];
	this->scale.y = matrix[1][1];
	this->scale.z = matrix[2][2];
	
}

JointTransformation::JointTransformation(DirectX::XMMATRIX transform)
{

	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, transform);

	float x, y, z, w;
	float diagonal = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];
	if (diagonal > 0)
	{
		float w4 = (float)(sqrtf(diagonal + 1.f) * 2.f);
		w = w4 / 4.f;
		x = (matrix.m[2][1] - matrix.m[1][2]) / w4;
		y = (matrix.m[0][2] - matrix.m[2][0]) / w4;
		z = (matrix.m[1][0] - matrix.m[0][1]) / w4;
	}
	else if ((matrix.m[0][0] > matrix.m[1][1]) && (matrix.m[0][0] > matrix.m[2][2]))
	{
		float x4 = (float)(sqrtf(1.f + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])*2.f);
		w = (matrix.m[2][1] - matrix.m[1][2]) / x4;
		x = x4 / 4.f;
		y = (matrix.m[1][0] + matrix.m[0][1]) / x4;
		z = (matrix.m[0][2] + matrix.m[2][0]) / x4;
	}
	else if (matrix.m[1][1] > matrix.m[2][2])
	{
		float y4 = (float)(sqrt(1.f + matrix.m[1][1] - matrix.m[0][0] - matrix.m[2][2])*2.f);
		w = (matrix.m[0][2] - matrix.m[2][0]) / y4;
		x = (matrix.m[1][0] + matrix.m[0][1]) / y4;
		y = y4 / 4.f;
		z = (matrix.m[2][1] + matrix.m[1][2]) / y4;
	}
	else
	{
		float z4 = (float)(sqrt(1.f + matrix.m[2][2] - matrix.m[0][0] - matrix.m[1][1])*2.f);
		w = (matrix.m[1][0] - matrix.m[0][1]) / z4;
		x = (matrix.m[0][2] + matrix.m[2][0]) / z4;
		y = z = (matrix.m[2][1] + matrix.m[1][2]) / z4;
		z = z4 / 4.f;
	}
	this->rotation.m128_f32[0] = x;
	this->rotation.m128_f32[1] = y;
	this->rotation.m128_f32[2] = z;
	this->rotation.m128_f32[3] = w;
	this->position.x = matrix.m[0][3];
	this->position.y = matrix.m[1][3];
	this->position.z = matrix.m[2][3];
	this->scale.x = matrix.m[0][0];
	this->scale.y = matrix.m[1][1];
	this->scale.z = matrix.m[2][2];



}

JointTransformation::JointTransformation(const JointTransformation & obj)
{
	this->position = obj.position;
	this->rotation = obj.rotation;
	this->scale= obj.scale;
}

JointTransformation::~JointTransformation()
{
}

void JointTransformation::operator=(const JointTransformation & obj)
{
	this->position = obj.position;
	this->rotation = obj.rotation;
	this->scale = obj.scale;
}

DirectX::XMMATRIX JointTransformation::getLocalTransform()
{
	//DirectX::XMMATRIX posMatrix = DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
	//DirectX::XMMATRIX quaternionM = DirectX::XMMatrixRotationQuaternion(this->rotation);
	//DirectX::XMMATRIX scMatrix = DirectX::XMMatrixScaling(this->scale.x,this->scale.y,this->scale.z);

	//testing affine matrix
	DirectX::XMVECTOR translationP	= DirectX::XMLoadFloat3(&this->position);
	DirectX::XMVECTOR rotationQ		= this->rotation;
	DirectX::XMVECTOR scaleS		= DirectX::XMLoadFloat3(&this->scale);
	DirectX::XMVECTOR origin		= DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.0f);
	DirectX::XMMATRIX affine		= DirectX::XMMatrixAffineTransformation(scaleS, origin, rotationQ, translationP);

	return affine;									  
}

//DirectX::XMMATRIX JointTransformation::quatToMat(DirectX::XMVECTOR q)
//{
//	double sqw = q.m128_f32[3] * q.m128_f32[3];
//	double sqx = q.m128_f32[0] * q.m128_f32[0];
//	double sqy = q.m128_f32[1] * q.m128_f32[1];
//	double sqz = q.m128_f32[2] * q.m128_f32[2];
//
//	// invs (inverse square length) is only required if quaternion is not already normalised
//	double invs = 1 / (sqx + sqy + sqz + sqw);
//	DirectX::XMFLOAT4X4 mat;
//	mat.m[0][0] = (sqx - sqy - sqz + sqw)*invs; // since sqw + sqx + sqy + sqz =1/invs*invs
//	mat.m[1][1] = (-sqx + sqy - sqz + sqw)*invs;
//	mat.m[2][2] = (-sqx - sqy + sqz + sqw)*invs;
//
//	double tmp1 = q.m128_f32[0] * q.m128_f32[1];
//	double tmp2 = q.m128_f32[2] * q.m128_f32[3];
//	mat.m[1][0] = 2.0 * (tmp1 + tmp2)*invs;
//	mat.m[0][1] = 2.0 * (tmp1 - tmp2)*invs;
//
//	tmp1 = q.m128_f32[0] * q.m128_f32[2];
//	tmp2 = q.m128_f32[1] * q.m128_f32[3];
//	mat.m[2][0] = 2.0 * (tmp1 - tmp2)*invs;
//	mat.m[0][2] = 2.0 * (tmp1 + tmp2)*invs;
//	tmp1 = q.m128_f32[1] * q.m128_f32[2];
//	tmp2 = q.m128_f32[0] * q.m128_f32[3];
//	mat.m[2][1] = 2.0 * (tmp1 + tmp2)*invs;
//	mat.m[1][2]= 2.0 * (tmp1 - tmp2)*invs;
//
//	
//	return DirectX::XMLoadFloat4x4(&mat);
//}

DirectX::XMFLOAT3 JointTransformation::getPosition() const
{
	return this->position;
}

DirectX::XMVECTOR JointTransformation::getRotation() const
{
	return this->rotation;
}

DirectX::XMFLOAT3 JointTransformation::getScale() const
{
	return this->scale;
}

//float JointTransformation::getPitch(DirectX::XMVECTOR Quaternion)
//{
//	return atan2(2 * (Quaternion.m128_f32[1]* Quaternion.m128_f32[2] + Quaternion.m128_f32[3] * Quaternion.m128_f32[0]), Quaternion.m128_f32[3] * Quaternion.m128_f32[3] - Quaternion.m128_f32[0] * Quaternion.m128_f32[0] - Quaternion.m128_f32[1] * Quaternion.m128_f32[1] + Quaternion.m128_f32[2] * Quaternion.m128_f32[2]);
//}
//
//float JointTransformation::getYaw(DirectX::XMVECTOR Quaternion)
//{
//	return asin(-2 * (Quaternion.m128_f32[0] * Quaternion.m128_f32[2] - Quaternion.m128_f32[3] * Quaternion.m128_f32[1]));
//}
//
//float JointTransformation::getRoll(DirectX::XMVECTOR Quaternion)
//{
//	return atan2(2 * (Quaternion.m128_f32[0] *Quaternion.m128_f32[1] + Quaternion.m128_f32[3] * Quaternion.m128_f32[2]), Quaternion.m128_f32[3] * Quaternion.m128_f32[3] + Quaternion.m128_f32[0] * Quaternion.m128_f32[0] - Quaternion.m128_f32[1] * Quaternion.m128_f32[1] - Quaternion.m128_f32[2] * Quaternion.m128_f32[2]);
//}

//JointTransformation JointTransformation::interpolate(JointTransformation frameA, JointTransformation frameB, float progression) //progression is 0 to 1
//{
//	DirectX::XMFLOAT3 pos = this->interpolate(frameA.position, frameB.position, progression);
//	DirectX::XMFLOAT4 pos4 = { pos.x, pos.y, pos.z, 0.f };
//	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionSlerp(frameA.rotation, frameB.rotation, progression);
//	//DirectX::XMFLOAT3 scale = this->interpolate(frameA.scale, frameB.scale, progression);
//	DirectX::XMFLOAT3 scale = { 1.f,1.f,1.f };
//	return JointTransformation(pos4, quaternion,scale);
//}

//JointTransformation JointTransformation::interpolate2(JointTransformation frameA, JointTransformation frameB, float progression)
//{
//	DirectX::XMFLOAT3 pos = this->interpolate2(frameA.position, frameB.position, progression);
//	DirectX::XMFLOAT4 pos4 = { pos.x, pos.y, pos.z, 0.f };
//	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionSlerp(frameA.rotation, frameB.rotation, progression);
//	DirectX::XMFLOAT3 scale = this->interpolate2(frameA.scale, frameB.scale, progression);
//
//	return JointTransformation(pos4, quaternion, scale);
//}
