#include "Animator.h"

void Animator::incrementAnimationTime(float deltaTime)
{
	this->animationTime += deltaTime;
	if (this->animationTime > currentAnimation.getLength())
	{
		this->animationTime = fmod(this->animationTime ,currentAnimation.getLength());
	}
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
	DirectX::XMMATRIX newMatrix;
	this->applyPoseToJoints(currentPos, entity.getRootJoint(), newMatrix);
	//}
}
