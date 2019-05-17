#include "Joint.h"

void Joint::calcInverseBindTransform(DirectX::XMMATRIX parentBindTransform)
{
	DirectX::XMMATRIX bindTransform = DirectX::XMMatrixMultiply(parentBindTransform, locaBindTransform);
	DirectX::XMVECTOR det= DirectX::XMMatrixDeterminant(bindTransform);
	this->inverseBindTransform = DirectX::XMMatrixInverse(&det,bindTransform);
	for (int i = 0; i < nrOfChildren; i++)
	{
		children[i]->calcInverseBindTransform(bindTransform);
	}

}

Joint::Joint()
{
	this->name = "Default";
	this->id = -1;

	this->nrOfChildren = 0;
	this->parent = nullptr;
	this->children = nullptr;
	this->locaBindTransform = {};
}

Joint::Joint(char * name, int id, int parentID, float invMatrixDiagonal)
{
	std::string newName(name);
	this->name = newName;
	this->id = id;

	DirectX::XMFLOAT4X4 invTemp;
	invTemp.m[0][0] = invMatrixDiagonal;
	invTemp.m[1][1] = invMatrixDiagonal;
	invTemp.m[2][2] = invMatrixDiagonal;
	invTemp.m[3][3] = invMatrixDiagonal;
	this->inverseBindTransform = DirectX::XMLoadFloat4x4(&invTemp);
}

Joint::Joint(std::string name, int id, int nrOfChildren, DirectX::XMMATRIX transform)
{
	this->name = name;
	this->id = id;
	this->nrOfChildren = nrOfChildren;
	this->parent = nullptr;

	this->children = new Joint*[nrOfChildren];
	for (int i = 0; i < nrOfChildren; i++)
	{
		this->children[i] = nullptr;
	}

	this->locaBindTransform = transform;
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

void Joint::init(std::string name, int id, int nrOf)
{
	this->name = name;
	this->id = id;
	this->nrOfChildren = nrOf;
	this->children = new Joint*[nrOfChildren];
	for (int i = 0; i < nrOfChildren; i++)
	{
		this->children[i] = nullptr;
	}
}

void Joint::setAnimationTransform(DirectX::XMMATRIX animationTransform)
{
	this->animatedTransform = animationTransform;
}

bool Joint::setParent(Joint *theJoint)
{
	//std::string test;
	//test = theJoint->getName();
	bool result = false;
	if (theJoint!=nullptr)
	{
		//for (int i = 0; i < theJoint->getNrOfChildren(); i++)
		//{
		//	if (theJoint->getChild(i) == nullptr)
		//	{
				this->parent = theJoint;
				//this->parent->setChildJoint(this, i);
				result = true;
			//}
		//}
	}
	return result;
}

bool Joint::setChildJoint(Joint * theJoint, int index)
{
	//std::string test;
	//test = theJoint->getName();
	bool result = false;
	if (theJoint!=nullptr)
	{
		this->children[index] = theJoint;
		//this->children[index]->setParent(this);
		//test = "NULL";
		//test = this->children[index]->getParent()->getName();
		result = true;
	}
	return result;;
}

DirectX::XMMATRIX Joint::getLocaBindTransform()
{
	return this->locaBindTransform;
}

DirectX::XMMATRIX Joint::getInverseBindTransform()
{
	return this->inverseBindTransform;
}

DirectX::XMMATRIX Joint::getAnimatedTransform()
{
	return this->animatedTransform;
}

int Joint::getNrOfChildren() const
{
	return this->nrOfChildren;
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

Joint * Joint::getChild(int index) const
{
	return this->children[index];
}
