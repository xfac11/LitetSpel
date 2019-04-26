#ifndef JOINT_H
#define JOINT_H
#include <DirectXMath.h>
class Joint
{
private:
//	Joint* *children; //eiter List or tree or linked list
	int id;
	DirectX::XMMATRIX transform;
public:
};
#endif // !JOINT_H
