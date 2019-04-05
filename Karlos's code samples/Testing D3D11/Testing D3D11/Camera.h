#ifndef CAMERA_H
#define CAMERA_H
#include <directxmath.h>
__declspec(align(16)) class Camera
{
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	//DirectX::XMFLOAT3 lookAt; 
	DirectX::XMMATRIX viewMatrix;
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

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Render();
	DirectX::XMMATRIX& GetViewMatrix();
};
#endif