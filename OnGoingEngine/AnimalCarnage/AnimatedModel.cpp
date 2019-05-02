#include "AnimatedModel.h"
#include "Animator.h"

void AnimatedModel::doAnimation(Animation *animation)
{
	animator->doAnimation(animation);
}

void AnimatedModel::update(float deltaTime)
{
	this->animator->update(deltaTime);
}

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

void AnimatedModel::addJointsToArray(Joint * headJoint, DirectX::XMMATRIX *& jointMatrices)
{
	jointMatrices[headJoint->getID()] = headJoint->getAnimatedTransform();
	for (int i = 0; i < headJoint->getNrOfChildren(); i++)
	{
		addJointsToArray(headJoint->getChild(i), jointMatrices);
	}
}

AnimatedModel::AnimatedModel()
{
	//this->animator = new Animator(*this);
}

AnimatedModel::~AnimatedModel()
{
	//delete this->animator;
	//delete model
	//delete texture 
}


bool AnimatedModel::setRootJoint()
{
	return false;
}

Joint * AnimatedModel::getRootJoint()
{
	return nullptr;
}

DirectX::XMMATRIX *& AnimatedModel::getJointTransforms(DirectX::XMMATRIX*& jointMatrices)
{
	jointMatrices = new DirectX::XMMATRIX[jointCount];
	addJointsToArray(root, jointMatrices);
	return jointMatrices;
}
