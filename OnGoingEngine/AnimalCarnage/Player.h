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
	GOLDEN,
	BROWN,
	GREY,
	CYAN,
	PINK,
	BLACK,
	WHITE
};
struct PlayerStats
{
	AnimalType type = FOX;
	PlayerColor color = RED;
	int playerID = 0;
	int kills = 0;
	int deaths = 0;
	int damageTaken = 0;
	int damageDealt = 0;
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
	string animName;
	bool animLoop;
	bool previousGrounded;
	float landingTimer;
	float landingLag;
	float jumpTimer;
	bool jumping;
	bool deathOfStone;

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
	PlayerStats stats;

	PlayerColor getColor() const;
	AnimalType getAnimalType() const;
	float getAnimSpeed() const;
	string getAnimName() const;
	bool getAnimLoop() const;
	float getJumpHeight() const;
	int getStrength() const;
	float getSpeed() const;
	float getWeight() const;
	int getMaxHealth() const;
	int getHealth() const;
	void reset();
	void takeDamage(int damage);
	bool isDead() const;
	void changeCharacter();
	bool getHit()const { return hit; }
	bool getHitStun();
	void setHitStun(bool hitStun);
	bool hit = false;
	//void groundedTimer();
	void setColorMask(PlayerColor color);
	void setAnimalType(AnimalType type);
	void setAnimalTypeAndMass(AnimalType type);
	
	DirectX::XMMATRIX sclMtx = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f);

	Player();
	~Player();
	void ResetRigidBody();
	bool canChange();
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
	void setDirection(int dir);
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
	bool getGrounded();
	void setCanWallJump(bool canWallJump);
	void addGroundMovingSpeed(XMFLOAT3 speed);
	void follow(XMFLOAT3 postion);
	GameObject* playerObj;

	float getPitch(DirectX::XMVECTOR Quaternion);
	float getYaw(DirectX::XMVECTOR Quaternion);
	float getRoll(DirectX::XMVECTOR Quaternion);

	void setDiedOfStone(bool death);
	bool getDiedOfStone();

	bool operator==(const Player& p);
};
#endif // ! PLAYER_H

