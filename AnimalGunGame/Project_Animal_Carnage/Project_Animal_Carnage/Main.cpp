#include <Windows.h>
#include "System.h"
#include <ctime>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::srand(static_cast <unsigned> (std::time(0)));
	System sys = System(hInstance, "Animal Carnage", nCmdShow);
	//sys.initialize();
	sys.run();//loop
	//sys.MessageHandler(hwnd);
	sys.shutDown();
	return (int)sys.getMsgWParam();
}