#include "Bear.h"
#include "Fox.h"
#include "GamePad.h"
#include "Primitives.h"
enum CHARACTER{
	BEAR,FOX
};

struct RumbleProperties
{
	float rumbleClock = 0;
	float rumbleTime = 0;
	DirectX::XMFLOAT2 rumble = XMFLOAT2(0, 0);
	bool rumbleEnabled = true;
};
struct Hitbox
{
	int totalTime;
	int time;
	GameObject* hitbox;
};
class Player
{
private:

	//player physics 
	bool isJumping;
	bool inAir;
	float airTimer;
	float jumpDir;
	bool grounded; //collision On ground
	bool canJump;
	float airSpeed;

	RumbleProperties theRumble;

	int dir;

	XMFLOAT3 Accleration = XMFLOAT3(0, 0, 0);
	XMFLOAT3 Velocity = XMFLOAT3(0, 0, 0);
	/*Primitives *CollisionShape;*/
public:
	Player();
	~Player();

	void initialize();
	void update(float dt, int id);
	bool updateRumble(float dt,int id);
	bool setRumble(bool rumble = true);

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
	AABB getAABB();

	Hitbox hitbox;

	GameObject* playerObj;
};

