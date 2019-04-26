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
	bool grounded; //collision On ground
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

	XMFLOAT3 Accleration = XMFLOAT3(0, 0, 0);
	XMFLOAT3 Velocity = XMFLOAT3(0, 0, 0);

public:
	Player();
	~Player();

	void initialize();
	void update(float dt, int id);
	void update(float dt);

	XMFLOAT3 mul(XMFLOAT3 l, float r);
	XMFLOAT3 add(XMFLOAT3 l, XMFLOAT3 r);
	void addForce(XMFLOAT3 Dir, float force);

	float magnitude(XMFLOAT3 l);
	XMFLOAT3 normalize(XMFLOAT3 l);
	float dot(const XMFLOAT3& l, const XMFLOAT3& r);
	XMFLOAT3 cross(const XMFLOAT3& l, XMFLOAT3& r);

	void move(float x, float y, float z);
	void move(XMFLOAT3 source);
	void setPosition(float x, float y, float z);
	void setScale(float x, float y, float z);
	XMFLOAT3 getPosition();

};

