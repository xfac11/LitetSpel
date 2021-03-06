#ifndef CAMERA_H
#define CAMERA_H
#include <vector>
#include <directxmath.h>
__declspec(align(16)) class Camera
{
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.f, 0.0f, 0.1f, 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0, 1.f, 0, 0.0);
	//DirectX::XMFLOAT3 lookAt; 
	DirectX::XMMATRIX viewMatrix;
	//DirectX::XMFLOAT3 acceleration;
	float accelerationX;
	float accelerationY;
	float accelerationZ;

	float tempPosX;
	float tempPosY;
	float tempPosZ;

	float medianX;
	float medianY;
	float medianZ;
public:
	Camera();
	~Camera();
	void* operator new(size_t i) //test
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetRotation(float x, float y, float z);
	void SetRotation(DirectX::XMFLOAT3 rotation);
	void move(float x, float y, float z);
	void move(DirectX::XMFLOAT3 position);
	void rotate(float x, float y, float z);
	void rotate(DirectX::XMFLOAT3 rotation);
	void cameraShake(float amount, DirectX::XMFLOAT3 direction);
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMVECTOR GetUp() const;
	void Render();
	DirectX::XMMATRIX& GetViewMatrix();
	void calcCamera(std::vector<DirectX::XMFLOAT3> playerPos);
};
#endif