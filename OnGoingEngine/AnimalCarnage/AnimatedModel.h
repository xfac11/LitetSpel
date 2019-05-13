#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H
//#include "Animator.h"
#include "Joint.h"
#include "Animation.h"
//#include "modelLoader.h"
class Animator; //forward-declaration
class AnimatedModel
{
private:
	//SKIN
	/*VAO ?   gameobject mesh
	float3 postion
	float2  uv;
	float3 normal;
	int3 jointID;
	float3 weight;

	*/
	//SKELETON	
	Joint* root;// jointHierarchy; //root of the joint structure
	int jointCount;

	Animator* animator;

	//setup joints
	void setupJoints();
	void addJointsToArray(Joint* headJoint, DirectX::XMMATRIX*& jointMatrices);
public:

	AnimatedModel();
	~AnimatedModel();
	bool setRootJoint();

	void doAnimation(Animation* animation);
	void update(float deltaTime);
	Joint* getRootJoint(); //make this point to root 
	DirectX::XMMATRIX*& getJointTransforms(DirectX::XMMATRIX*& jointMatrices);
};
#endif // !AnimatedModel_H
