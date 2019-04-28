#include "AnimatedModel.h"

AnimatedModel::AnimatedModel()
{
	this->animator = new Animator;
}

AnimatedModel::~AnimatedModel()
{
	delete this->animator;
}

void AnimatedModel::update(float deltaTime)
{
	this->animator->update(deltaTime);
}
