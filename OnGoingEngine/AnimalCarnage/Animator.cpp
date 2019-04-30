#include "Animator.h"

void Animator::incrementAnimationTime(float deltaTime)
{
	this->animationTime += deltaTime;
	if (this->animationTime > currentAnimation.getLength())
	{
		this->animationTime = fmod(this->animationTime ,currentAnimation.getLength());
	}
}

std::map<std::string, DirectX::XMMATRIX> Animator::calculateCurrentAnimationPose()
{
	return std::map<std::string, DirectX::XMMATRIX>();
}

void Animator::applyPoseToJoints(std::map<std::string, DirectX::XMMATRIX> currentPos, Joint * joint, DirectX::XMMATRIX parentTransform)
{
}

Animator::Animator()
{
}

void Animator::doAnimation(Animation animation)
{
	this->animationTime = 0;
	this->currentAnimation = animation;
}

void Animator::update(float deltaTime)
{
	//currentAnimation.
	//if (currentAnimation != nullptr)
	//{
	this->incrementAnimationTime(deltaTime);
	std::map<std::string, DirectX::XMMATRIX> currentPos = this->calculateCurrentAnimationPose();
	DirectX::XMMATRIX newMatrix = {};
	this->applyPoseToJoints(currentPos, entity.getRootJoint(), newMatrix);
	//}
}
