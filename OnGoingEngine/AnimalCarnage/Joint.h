#ifndef JOINT_H
#define JOINT_H
#include <DirectXMath.h>
#include "Luna/include/Luna/Luna.h"
#include <string>
class Joint
{
private:
	std::string name;
	int id;
	DirectX::XMMATRIX inverseBindTransform;
	Joint *parent;

public:
	Joint();
	void operator=(const Luna::Joint& obj);
	bool setParent(Joint * theJoint);

	DirectX::XMMATRIX getInverseBindTransform();
	int getID()const;
	std::string getName()const;
	Joint* getParent()const;
	
};
#endif // !JOINT_H
