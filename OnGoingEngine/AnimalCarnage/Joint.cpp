#include "Joint.h"

Joint::Joint()
{
	this->name = "Default";
	this->id = -1;
	this->parent = nullptr;
	this->inverseBindTransform = {};
}


void Joint::operator=(const Luna::Joint & obj)
{
	std::string theName(obj.jointName);
	this->name = theName;
	this->id = obj.jointID;
	this->parent = nullptr;
	DirectX::XMFLOAT4X4 invTemp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			invTemp.m[i][j] = obj.invBindposeMatrix[i][j];
		}
	}
	this->inverseBindTransform = DirectX::XMLoadFloat4x4(&invTemp);
}

bool Joint::setParent(Joint *theJoint)
{
	bool result = false;
	if (theJoint!=nullptr)
	{
		this->parent = theJoint;	
		result = true;
	}
	return result;
}

DirectX::XMMATRIX Joint::getInverseBindTransform()
{
	return this->inverseBindTransform;
}

int Joint::getID() const
{
	return this->id;
}

std::string Joint::getName() const
{
	return this->name;
}

Joint * Joint::getParent() const
{
	return this->parent;
}
