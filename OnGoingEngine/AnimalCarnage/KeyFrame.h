#ifndef KEYFRAME_H
#define KEYFRAME_H
#include "JointTransformation.h"
#include "Luna/include/Luna/Luna.h"
class KeyFrame
{
private:
	float timeStamp;
	JointTransformation transform;
public:
	KeyFrame();
	~KeyFrame();
	void operator=(const Luna::Keyframe& obj);


	float getLenght()const;
	JointTransformation & getJointKeyFrames();
};
#endif