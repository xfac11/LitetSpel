#ifndef PLAYER_H
#define PLAYER_H

#include "GamePad.h"
#include "GameObject.h"
#include "Animal.h"
#include <ctime>
//#include "Primitives.h"

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
enum PlayerColor
{
	RED,
	BLUE,
	GREEN,
	YELLOW,
	CYAN,
	PINK,
	BLACK,
	WHITE
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
	bool canPressJump;
	float airSpeed;
	float facing;
	bool canWallJump;
	bool wallJumpReset;
	bool canDoubleJump;
	bool hitStun;
	int hitTimer;
	bool punching;
	int hitTime2;
	int hitTime;
	int health;
	bool canPressPunch;
	int deathTimer;
	int groundTimer;
	float animSpeed;

	bool canBeAnimal[4];
	int randomNumberArray[4] = {0,1,2,3};
	int nextAnimal;

	RumbleProperties theRumble;
	PlayerColor color;
	AnimalType type;
	AnimalType ArrayOfAnimals[4];
	int currentAnimal;
	XMFLOAT3 Accleration = XMFLOAT3(0, 0, 0);
	XMFLOAT3 Velocity = XMFLOAT3(0, 0, 0);

	/*Primitives *CollisionShape;*/
public:
	float getAnimSpeed() const;
	float getJumpHeight() const;
	int getStrength() const;
	float getSpeed() const;
	float getWeight() const;
	int getMaxHealth() const;
	int getHealth() const;
	void takeDamage(int damage);
	bool isDead() const;
	void setColorMask(DirectX::XMFLOAT4 color);
	void changeCharacter();
	bool getHit()const { return hit; }
	bool getHitStun();
	void setHitStun(bool hitStun);
	bool hit = false;
	//void groundedTimer();
	
	Player();
	~Player();

	void initialize(AnimalType type, PlayerColor color);
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
	void setRigidbodyPosition(float x, float y, float z);
	void setScale(float x, float y, float z);
	XMFLOAT3 getPosition();
	AABB getAABB();

	Hitbox hitbox;
	int dir;

	int getNextAnimal();
	void setGrounded(bool grounded);
	void setCanWallJump(bool canWallJump);
	void addGroundMovingSpeed(XMFLOAT3 speed);
	void follow(XMFLOAT3 postion);
	GameObject* playerObj;

	float getPitch(DirectX::XMVECTOR Quaternion);
	float getYaw(DirectX::XMVECTOR Quaternion);
	float getRoll(DirectX::XMVECTOR Quaternion);
};


#endif // ! PLAYER_H

