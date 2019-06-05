#ifndef ANIMATION_H
#define ANIMATION_H
#include "KeyFrame.h"
#include <vector>
class Animation
{
private:
	std::string name;
	float fps; //rate
	float duration;
	std::vector<std::vector<KeyFrame>> keyframePack;
	int nrOfJoints;
	int nrOfKeyFrames;

public:
	Animation();
	Animation(float fps, float duration, std::string name, int jointCount, int keyframeCount);
	void operator=(const Animation & obj);
	void setKeyframes(std::vector<std::vector<Luna::Keyframe>> & newFrames);
	std::vector<std::vector<KeyFrame>>& getKeyframes();
	std::string getName()const;
	float getFPS()const;
	float getDuration()const;
};
#endif // !ANIMATION_H
