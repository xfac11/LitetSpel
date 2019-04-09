#ifndef SYSTEM_H
#define SYSTEM_H
#include <math.h>
#include <d3d11.h>
#include <DirectXMath.h>
class System
{
private:
public:
	System(HINSTANCE hInstance, LPCSTR name, int nCmdShow);
	~System();

	bool initialize();
	void run();
	void shutDown();
	WPARAM getMsgWParam();
}
#endif


//game loop 
//keyboard
//mouse