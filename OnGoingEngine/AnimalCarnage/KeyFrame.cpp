#include "KeyFrame.h"

KeyFrame::KeyFrame()
{
	this->timeStamp = 0.f;
	float temp[4][4] = {};
	this->transform = JointTransformation(temp);
}

KeyFrame::~KeyFrame()
{
}

void KeyFrame::operator=(const Luna::Keyframe & obj)
{
	this->timeStamp = obj.timePosition;

	float temp[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp[i][j] = obj.keyframeMatrix[i][j];
		}
	}
	this->transform = JointTransformation(temp);
}
