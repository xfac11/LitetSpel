#include <Windows.h>
#include "System.h"
//#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dcompiler.lib")
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	System sys = System(hInstance, "coolagubben", nCmdShow);
	//sys.initialize();
	sys.run();//loop
	//sys.MessageHandler(hwnd);
	sys.shutDown();
	return (int)sys.getMsgWParam();
}