#include "AnimatedModel.h"

//AnimatedModel::AnimatedModel()
//{
//	this->animator = new Animator;
//}
//
//AnimatedModel::~AnimatedModel()
//{
//	delete this->animator;
//}
//
//void AnimatedModel::update(float deltaTime)
//{
//	this->animator->update(deltaTime);
//}

void AnimatedModel::setupJoints()
{
	//name
	//id
	//parent id
	//matrix


	//make a array of Joint pointers
	//link parent node to heirchy 

	//no need to setup root, it's nullptr
	//somehow get nrOf children or use default constructor
	//joint[1]->Joint(name, id, 3, matrix); 
	//joint[1]->setParent(joints[joint[1].parentID])
}

AnimatedModel::AnimatedModel()
{
}

AnimatedModel::~AnimatedModel()
{
}

bool AnimatedModel::setRootJoint()
{
	return false;
}

Joint * AnimatedModel::getRootJoint()
{
	return nullptr;
}
