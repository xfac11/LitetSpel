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
	
public:
	JointTransformation();
	JointTransformation(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 rotation, DirectX::XMFLOAT4 scale); //Pos: xyz, Quaternion: xyzw
	JointTransformation(DirectX::XMFLOAT4 position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 scale);
	JointTransformation(float matrix[4][4]);
	JointTransformation(const JointTransformation& obj);
	~JointTransformation();

	void operator=(const JointTransformation& obj);
	DirectX::XMMATRIX getLocalTransform();

	DirectX::XMFLOAT3 & getPosition(); //positon xyz w
	DirectX::XMVECTOR & getRotation(); //quaternion xyzw
	DirectX::XMFLOAT3 & getScale();

};
#endif // !JOINTTRANSFORMATION
