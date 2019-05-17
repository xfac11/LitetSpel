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
	DirectX::XMMATRIX locaBindTransform;
	DirectX::XMMATRIX inverseBindTransform;
	DirectX::XMMATRIX animatedTransform;


	Joint *parent;
	Joint* *children; //eiter List or tree or linked list
	int nrOfChildren;
	
protected:
	void calcInverseBindTransform(DirectX::XMMATRIX parentBindTransform);
public:
	Joint();
	Joint(char* name, int id, int parentID, float invMatrixDiagonal);
	Joint(char* name, int id, int parentID, DirectX::XMMATRIX invBindposeMatrix);
	Joint(std::string name, int id, int nrOfChildren, DirectX::XMMATRIX transform);
	//Joint(const Joint& obj);
	void operator=(const Luna::Joint& obj);

	void init(std::string name,int id, int nrOf);

	void setAnimationTransform(DirectX::XMMATRIX animationTransform);


	bool setParent(Joint * theJoint);
	bool setChildJoint(Joint * theJoint, int index);

	DirectX::XMMATRIX getLocaBindTransform();
	DirectX::XMMATRIX getInverseBindTransform();
	DirectX::XMMATRIX getAnimatedTransform();

	int getNrOfChildren()const;
	int getID()const;
	std::string getName()const;

	Joint* getParent()const;
	Joint* getChild(int index)const;
};
#endif // !JOINT_H
