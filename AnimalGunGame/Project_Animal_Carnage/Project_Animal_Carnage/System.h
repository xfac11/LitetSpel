#ifndef SYSTEM_H
#define SYSTEM_H
#include <math.h>
#include <d3d11.h>
#include <DirectXMath.h>
class System
{
private:
	//Statemachine* statemachine;
	//static Keyboard* keboard;
	//static Mouse* mouse;
	//static D3D* d3d;
public:
	System(HINSTANCE hInstance, LPCSTR name, int nCmdShow);
	~System();
	
	bool initialize();
	void run();
	void shutDown();
	WPARAM getMsgWParam();
	//static bool isKeyPressed(char* key);
}
#endif


//game loop 
//keyboard
//mouse