#ifndef JOINTTRANSFORMATION_H
#define JOINTTRANSFORMATION_H
#include <DirectXMath.h>
#include <string>
class JointTransformation
{
private:
	//std::string name;
	DirectX::XMFLOAT3 position; //positon xyz w
	DirectX::XMVECTOR rotation; //quaternion xyzw
	//FLOAT4 scale 

	DirectX::XMFLOAT3 interpolate(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float progression);
public:
	JointTransformation();
	JointTransformation(DirectX::XMFLOAT3 position, DirectX::XMVECTOR rotation); //Pos: xyz, Quaternion: xyzw
	JointTransformation(float matrix[4][4]);
	JointTransformation(const JointTransformation& obj);
	~JointTransformation();

	void operator=(const JointTransformation& obj);
	DirectX::XMMATRIX getLocalTransform();

	JointTransformation interpolate(JointTransformation frameA, JointTransformation frameB, float progression);
};
#endif // !JOINTTRANSFORMATION
