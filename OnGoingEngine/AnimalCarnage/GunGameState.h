#ifndef GUN_GAME_STATE_H
#define GUN_GAME_STATE_H
#include "State.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <DirectXMath.h>

class GunGameState : public State
{
private:
	int nrOfPlayers;

	Player** player;

	//Player* player[4];

	//rumble
	//struct RumbleTemp
	//{
	//	float rumbleClock;
	//	float rumbleTime;
	//	DirectX::XMFLOAT2 rumble;
	//};
	//RumbleTemp theRumble[4];

	//Temporary player stuct
	/*struct PlayerTemp
	{
		bool isJumping;
		bool inAir;
		float airTimer;
		float jumpDir; 
		bool grounded;
		bool canJump;
		float airSpeed;
		DirectX::XMFLOAT3 direction;
	};
	PlayerTemp tplayer[4];*/



	struct ItemTemp
	{
		bool isFlying;
		float airTimer;
		bool grounded;
		int lastDir;
		float weight;//
		DirectX::XMFLOAT3 direction;
	};
	ItemTemp items[2];

	//void inPause();
	bool checkReset(DirectX::GamePad::State state);

public:
	GunGameState();
	~GunGameState();

	bool initailize();
	bool render();
	void renderImgui();
	bool update(float deltaTime);
	void shutDown();

	bool controllerIsConnected(int controllerPort);

	//Temporary
	//box and box
	//bool collision(DirectX::XMFLOAT2 posOne, DirectX::XMFLOAT2 scaleOne,int playerID, DirectX::XMFLOAT2 posTwo, DirectX::XMFLOAT2 scaleTwo, int itemID);
	//circle and circle
	bool collision(DirectX::XMFLOAT2 posOne, float radiusOne, DirectX::XMFLOAT2 posTwo, float radiusTwo);
	//box and circle
	bool collision(DirectX::XMFLOAT2 posBox, DirectX::XMFLOAT2 scaleBox, DirectX::XMFLOAT2 posCircle, float radiusCircle);


	
};

#endif // !GUN_GAME_STATE_H
