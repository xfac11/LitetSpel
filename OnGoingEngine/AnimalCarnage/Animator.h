#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "Animation.h"
#include "AnimatedModel.h"
#include <map>
class Animator
{
private:
	AnimatedModel entity;

	Animation currentAnimation;
	float animationTime;

	void incrementAnimationTime(float deltaTime);
	
	std::map<std::string, DirectX::XMMATRIX> calculateCurrentAnimationPose();
	void applyPoseToJoints(std::map<std::string, DirectX::XMMATRIX> currentPos, Joint *joint, DirectX::XMMATRIX parentTransform);
public:
	Animator();

	void doAnimation(Animation animation);
	void update(float deltaTime);
};
#endif // !ANIMATOR_H
