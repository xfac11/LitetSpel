#include "Bear.h"
#include "GamePad.h"
enum CHARACTER{
	BEAR,FOX
};
class Player
{
private:
	//ska den hålla i en animal eller flera
	//GamePad* gamePad;


	//player physics 
		bool isJumping;
	bool inAir;
	float airTimer;
	float jumpDir;
	bool grounded;
	bool canJump;
	float airSpeed;


	struct RumbleProperties
	{
		float rumbleClock;
		float rumbleTime;
		DirectX::XMFLOAT2 rumble;
		bool rumbleEnabled;
	};

	GameObject* playerObj;
public:
	Player();
	~Player();

	void initialize();
	void update(float dt, int id);
};

