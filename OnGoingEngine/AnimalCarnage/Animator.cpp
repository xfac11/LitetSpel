#include "Animator.h"

void Animator::incrementAnimationTime(float deltaTime)
{
	//this->animationTime += deltaTime;
	//if (this->animationTime > currentAnimation->getLength())
	//{
	//	this->animationTime = fmod(this->animationTime ,currentAnimation->getLength());
	//}
}

std::map<std::string, DirectX::XMMATRIX> Animator::calculateCurrentAnimationPose()
{
	return std::map<std::string, DirectX::XMMATRIX>();
}

void Animator::applyPoseToJoints(std::map<std::string, DirectX::XMMATRIX> currentPose, Joint * joint, DirectX::XMMATRIX parentTransform)
{
	DirectX::XMMATRIX currentLocalTransform = currentPose.find(joint->getName())->second;
	DirectX::XMMATRIX currentTransform = DirectX::XMMatrixMultiply(parentTransform, currentLocalTransform);
	for (int i = 0; i < joint->getNrOfChildren(); i++)
	{
		applyPoseToJoints(currentPose, joint->getChild(i), currentTransform);
	}
	currentTransform = DirectX::XMMatrixMultiply(currentTransform, joint->getInverseBindTransform());
	joint->setAnimationTransform(currentTransform);
}

Animator::Animator(AnimatedModel entity)
{
	this->entity = entity;
	//this->currentAnimation = nullptr;
}

void Animator::doAnimation(Animation* animation)
{
	this->animationTime = 0;
	this->currentAnimation = animation;
}

void Animator::update(float deltaTime)
{
	
	if (currentAnimation != nullptr)
	{
	this->incrementAnimationTime(deltaTime);
	std::map<std::string, DirectX::XMMATRIX> currentPos = this->calculateCurrentAnimationPose();
	DirectX::XMMATRIX newMatrix = {};
	this->applyPoseToJoints(currentPos, entity.getRootJoint(), newMatrix);
	}
}
