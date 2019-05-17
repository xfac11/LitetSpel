#ifndef JOINTTRANSFORMATION_H
#define JOINTTRANSFORMATION_H
#include <DirectXMath.h>
#include <string>
class JointTransformation
{
private:
	
	DirectX::XMFLOAT3 position; //positon xyz w
	DirectX::XMVECTOR rotation; //quaternion xyzw
	DirectX::XMFLOAT3 scale;

	DirectX::XMFLOAT3 interpolate(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float progression);
	DirectX::XMFLOAT3 interpolate2(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float progression);
public:
	JointTransformation();
	JointTransformation(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 rotation, DirectX::XMFLOAT4 scale); //Pos: xyz, Quaternion: xyzw
	JointTransformation(DirectX::XMFLOAT4 position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 scale);
	JointTransformation(float matrix[4][4]);
	JointTransformation(DirectX::XMMATRIX transform);
	JointTransformation(const JointTransformation& obj);
	~JointTransformation();

	void operator=(const JointTransformation& obj);
	DirectX::XMMATRIX getLocalTransform();

	JointTransformation interpolate(JointTransformation frameA, JointTransformation frameB, float progression);
	JointTransformation interpolate2(JointTransformation frameA, JointTransformation frameB, float progression);
};
#endif // !JOINTTRANSFORMATION
