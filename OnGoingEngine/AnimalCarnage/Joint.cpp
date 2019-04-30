#include "Joint.h"

Joint::Joint()
{
	this->name = "Default";
	this->id = -1;

	this->nrOfChildren = 0;
	this->parent = nullptr;
	this->children = nullptr;
	this->locaBindTransform = {};
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
