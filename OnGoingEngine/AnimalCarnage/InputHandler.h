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
		bool rumble;
		bool Active;
	};
	DirectX::GamePad theGamePad;
	DirectX::GamePad::ButtonStateTracker tracker;
	DirectX::XMFLOAT3 direction[4];
	PlayerInput player[4];


	DirectX::XMFLOAT3 cameraRotation;

	//rumble
	float rumbleClock;
	float rumbleTime;
	DirectX::XMFLOAT2 rumble;

	//Jump physics
	bool isJumping;
	bool inAir;
	float airTimer;
	float jumpDir; //

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

	DirectX::XMFLOAT3 getDirection(int player);
	bool controllerIsConnected(int controllerPort);

	void inGameMode(float deltaTime, bool grounded);

	//box and box
	bool collision(DirectX::XMFLOAT2 posOne, DirectX::XMFLOAT2 scaleOne, DirectX::XMFLOAT2 posTwo, DirectX::XMFLOAT2 scaleTwo);
	//circle and circle
	bool collision(DirectX::XMFLOAT2 posOne, float radiusOne, DirectX::XMFLOAT2 posTwo, float radiusTwo);
	//box and circle
	bool collision(DirectX::XMFLOAT2 posBox, DirectX::XMFLOAT2 scaleBox, DirectX::XMFLOAT2 posCircle, float radiusCircle);

};
#endif // INPUTHANDLER_H