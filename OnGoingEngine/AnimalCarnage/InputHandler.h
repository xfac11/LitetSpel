#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include "Keyboard.h"
#include "Mouse.h"
#include "GamePad.h"
#include <DirectXMath.h>
class InputHandler
{
private:
	struct PlayerInput
	{
		int id;
		bool Active;
	};
	DirectX::GamePad theGamePad;
	DirectX::GamePad::ButtonStateTracker tracker;
	DirectX::XMFLOAT3 direction[4];
	DirectX::XMFLOAT3 cameraPosition;
	DirectX::XMFLOAT3 cameraRotation;
	PlayerInput player[4];

	//rumble
	float rumbleClock;
	float rumbleTime;
	DirectX::XMFLOAT2 rumble;

	//Jump physics
	bool isJumping;
	float jumpTimer;

	//int nrOfPlayers;

	bool checkReset(int player);
	void goToStartScreen();

	void updateController(int controllerPort);
	void inPauseMode(int id);
	void inSelectionMode();
	void inMenuMode();
	void inResultScreen();
	void inRumbleMode();
public:
	InputHandler();
	~InputHandler();


	DirectX::XMFLOAT3 getCameraPos();
	DirectX::XMFLOAT3 getCameraRot();
	DirectX::XMFLOAT3 getDirection(int player);
	bool controllerIsConnected(int controllerPort);

	void inGameMode(float deltaTime, bool grounded);

	float calcCamera(DirectX::XMFLOAT3 posLeft, DirectX::XMFLOAT3 posRight);
};
#endif // INPUTHANDLER_H