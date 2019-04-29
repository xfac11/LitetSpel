#ifndef ANIMATION_H
#define ANIMATION_H
#include "JointAnimation.h"
class Animation
{
private:
	//keyframes??
	JointAnimation** anims;
	int nrOfJoints;
public:
	Animation();
	void operator=(const Animation & obj);

	float getLength()const;
};
#endif // !ANIMATION_H
