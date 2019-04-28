#ifndef JOINT_H
#define JOINT_H
#include <DirectXMath.h>
#include <string>
class Joint
{
private:
	std::string name;
	int id;
	DirectX::XMMATRIX locaBindTransform;
	DirectX::XMMATRIX inverseBindTransform;
	DirectX::XMMATRIX animatedTransform;
	//have int id or pointer list system??
	//int parent;
	//int** children;
	


	Joint *parent;
	Joint* *children; //eiter List or tree or linked list
	int nrOfChildren;
	
	
public:
	Joint();
	Joint(std::string name, int id, int nrOfChildren, DirectX::XMMATRIX transform);

	bool setParent(Joint * theJoint);
	bool setChildJoint(Joint * theJoint, int index);

	DirectX::XMMATRIX getAnimatedTransform();
};
#endif // !JOINT_H
