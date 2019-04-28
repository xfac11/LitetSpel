#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H
//#include "Animator.h"
#include "Joint.h"
class AnimatedModel
{
private:
	Joint jointHierarchy; //root of the joint structure

	/*VAO ?   gameobject mesh
	float3 postion
	float2  uv;
	float3 normal;
	float3 tangent;
	float3 binormal;
	int3 jointID;
	float3 weight;
	
	
	*/
public:

	AnimatedModel();

	Joint getRootJoint(); //make this point to root 
};
#endif // !AnimatedModel_H
