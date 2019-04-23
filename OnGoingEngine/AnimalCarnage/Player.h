#include "Bear.h"
#include "GamePad.h"
enum CHARACTER{
	BEAR,FOX
};
class Player
{
private:
	//ska den hålla i en animal eller flera
	Animal animal[4];
	//GamePad* gamePad;
	int current;


	//player physics 
	bool isJumping;
	bool inAir;
	float airTimer;
	float jumpDir;
	bool grounded;
	DirectX::XMFLOAT3 direction;


	struct RumbleProperties
	{
		float rumbleClock;
		float rumbleTime;
		DirectX::XMFLOAT2 rumble;
		bool rumbleEnabled;
	};

	
public:
	Player();
	~Player();
	void SelectCharacter(CHARACTER b = BEAR);
	void update();
	AABB GetBody();

};

