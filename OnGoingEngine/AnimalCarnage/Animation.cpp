#include "Animation.h"

Animation::Animation()
{
	this->fps = 60.f;
	this->duration = 0;
	this->name = "Default";
	this->nrOfJoints = 0;
	this->nrOfKeyFrames = 0;
}

Animation::Animation(float fps, float duration, std::string name, int jointCount, int keyframeCount)
{
	this->fps = fps;
	this->duration = duration;
	this->name = name;
	this->nrOfJoints = jointCount;
	this->nrOfKeyFrames = keyframeCount;
}

void Animation::operator=(const Animation & obj)
{
	this->fps = obj.fps;
	this->duration = obj.duration;
	this->name = obj.name;
	this->nrOfJoints = obj.nrOfJoints;
	this->nrOfKeyFrames = obj.nrOfKeyFrames;
	
	std::vector<KeyFrame> tempKF;
	tempKF.resize(nrOfKeyFrames);
	for (int i = 0; i < nrOfJoints; i++)
	{
		//tempKF = newFrames[i];
		for (int j = 0; j < nrOfKeyFrames; j++)
		{
			tempKF[j] = obj.keyframePack[i][j];
		}
		this->keyframePack.push_back(tempKF);
	}

}

void Animation::setKeyframes(std::vector<std::vector<Luna::Keyframe>> & newFrames)
{
	std::vector<KeyFrame> tempKF;
	tempKF.resize(nrOfKeyFrames);
	for (int i = 0; i < nrOfJoints; i++)
	{
		//tempKF = newFrames[i];
		for (int j = 0; j < nrOfKeyFrames; j++)
		{
			tempKF[j] = newFrames[i][j];
		}
		this->keyframePack.push_back(tempKF);
	}
	
}

std::vector<std::vector<KeyFrame>>& Animation::getKeyframes()
{
	return this->keyframePack;
}

std::string Animation::getName() const
{
	return this->name;
}

float Animation::getFPS() const
{
	return this->fps;// length;
}

float Animation::getDuration() const
{
	return this->duration;
}
