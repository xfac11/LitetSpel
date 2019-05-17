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


	DirectX::XMFLOAT4 position	= DirectX::XMFLOAT4(obj.translation[0], obj.translation[1], obj.translation[2], obj.translation[3]);
	DirectX::XMFLOAT4 rotation	= DirectX::XMFLOAT4(obj.rotation[0], obj.rotation[1], obj.rotation[2], obj.rotation[3]);
	DirectX::XMFLOAT4 scale		= DirectX::XMFLOAT4(obj.scale[0], obj.scale[1], obj.scale[2], obj.scale[3]);

	this->transform = JointTransformation(position, rotation, scale);
}

float KeyFrame::getLenght() const
{
	return this->timeStamp;
}

JointTransformation KeyFrame::getJointKeyFrames() const
{
	return this->transform;
}
