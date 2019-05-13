#ifndef ANIMATION_H
#define ANIMATION_H
#include "KeyFrame.h"
class Animation
{
private:
	std::string name;
	float fps; //rate
	float duration;
	KeyFrame** frame;
	int nrOfKeyFrames;

public:
	Animation();
	void operator=(const Animation & obj);

	float getLength()const;
};
#endif // !ANIMATION_H
