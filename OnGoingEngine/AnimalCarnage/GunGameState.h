#ifndef GUN_GAME_STATE_H
#define GUN_GAME_STATE_H

#include "State.h"
//#include "GamePad.h"
class GunGameState : public State
{
private:
	//rumble
	struct RumbleTemp
	{
		float rumbleClock;
		float rumbleTime;
		DirectX::XMFLOAT2 rumble;
	};
	RumbleTemp theRumble[4];

	//Temporary player stuct
	struct PlayerTemp
	{
		bool isJumping;
		bool inAir;
		float airTimer;
		float jumpDir; 
		bool grounded;
		DirectX::XMFLOAT3 direction;
	};
	PlayerTemp player[4];

	void updateRumble(float deltaTime, int id);
public:
	GunGameState();
	~GunGameState();

	bool initailize();
	bool render();
	bool update(float deltaTime);
	void shutDown();

	DirectX::XMFLOAT3 getDirection(int id);
	bool controllerIsConnected(int controllerPort);
};

#endif // !GUN_GAME_STATE_H
