#ifndef GUN_GAME_STATE_H
#define GUN_GAME_STATE_H
#include "State.h"
#include "Player.h"
#include <iostream>
#include <string>
#include"Geometry.h"
#include <algorithm>
#include <DirectXMath.h>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "Objects.h"

class GunGameState : public State
{
private:
	int nrOfPlayers;
	Player** player;

	GuiBase* inGameGui;
	GuiBase* pauseGui;
	bool paused;

	//btDynamicsWorld* world;
	//btDispatcher* dispatcher;
	//btBroadphaseInterface* broadphase;
	//btConstraintSolver* solver;
	//btCollisionConfiguration* collisionConfig;

	////vector for rigid body
	//std::vector<btRigidBody*> bodies;
	//btRigidBody* addSphere(float rad, float x, float y, float z, float mass);

	//void renderSphere(btRigidBody* sphere) {
	//	if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE) {
	//		return;
	//	}
	//	float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
	//	btTransform t;
	//	sphere->getMotionState()->getWorldTransform(t);
	//}
	bool testColBox;

	//void inPause();
	bool checkReset(DirectX::GamePad::State state);

	GameObject* ground;
	GameObject* wall1;
	GameObject* wall2;

	Objects * object[4];


public:
	GunGameState();
	~GunGameState();

	void pause(bool paused);
	int getNrOfPlayers() const;
	Player* getPlayer(int id) const;

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
	//bool collision(DirectX::XMFLOAT2 posOne, float radiusOne, DirectX::XMFLOAT2 posTwo, float radiusTwo);
	////box and circle
	//bool collision(DirectX::XMFLOAT2 posBox, DirectX::XMFLOAT2 scaleBox, DirectX::XMFLOAT2 posCircle, float radiusCircle);



};

#endif // !GUN_GAME_STATE_H
