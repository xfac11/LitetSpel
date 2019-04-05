#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <string>
#include <math.h> 
#include "D3D.h"
#include "Camera.h"
#include "GBuffer.h"
#include "LightShader.h"
#include "FullscreenQuad.h"
#include "Model.h"
#include "Triangle.h"
#include "Terrain.h" 
#include "particlesystem.h"
#include "SkyBox.h"
//#include "bth_image.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
__declspec(align(16)) class Graphics
{
private:
	D3D* Direct3D;
	Camera* theCamera;
	Model* *theModel;
	//ColorShader* theColorShader[2];//forward

	Terrain* theTerrain;
	ParticleSystem* particles;
	SkyBox* theSky;

	//defered render
	DeferedShader* dShader;
	LightShader* lShader;
	GBuffer* gBuffer;
	FullscreenQuad* fullQuad;

	int cap; //models cap

	float jumpTimer;
	bool isJumping;
	float height;

	float color[4]; //scene class
	DirectX::XMFLOAT3 camPos; //scene class
	DirectX::XMFLOAT3 camRot; //scene class
	float gIncrement; //scene class

	//fron to back render holder
	int modelDistArray[4];

	DirectX::XMVECTOR rotaxis;
	int texToShow;
	bool freezeCheck;
	DirectX::XMFLOAT3 cullingPos; //freezed culling position
	std::string mouseObject;
	bool beginCheck;
	int daScreenWidth;
	int daScreenHeight;

	void renderImgui();
	bool render(); //float [4]color
	void renderToTexture();
	void deferredRender();

	void intersectionTest(int posX, int posY, DirectX::XMFLOAT3 ObjPosition,float radius);
	bool sphereRayIntersection(DirectX::XMFLOAT3 rayOrigin, DirectX::XMFLOAT3 rayDirection, DirectX::XMFLOAT3 pos, float radius);
	bool boxRayIntersection(DirectX::XMFLOAT3 rayOrigin, DirectX::XMFLOAT3 rayDirection, DirectX::XMFLOAT3 center, DirectX::XMFLOAT4 u_hu, DirectX::XMFLOAT4 v_hv, DirectX::XMFLOAT4 w_hw);
	//bool triangleIntersection();
	int selectionSort();
public:
	Graphics();
	~Graphics();
	void* operator new(size_t i) //test
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
	void initImgui(HWND & hWnd);
	void move(Direction forward, Direction left_right, Direction up_down, bool flyMode, int mouseX, int mouseY, 
		int mousePosX, int mousePosY, bool mouseLeft);// Direction forward, Direction left_Right, Direction, Direction upDown);
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);

	void Shutdown();
	bool Frame(bool move1, bool move2);

};
#endif