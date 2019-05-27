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
#include <ctime>

class GunGameState : public State
{
private:
	int nrOfPlayers;
	Player** player;

	GuiBase* inGameGui;
	GuiBase* pauseGui;
	bool paused;
	int cameraFocus;
	int objectId;
	btVector3 spawnPoints[4];
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


	//bool checkReset(DirectX::GamePad::State state);

	GameObject* ground;
	GameObject* wall1;
	GameObject* wall2;

	//static GunGameState * shelf;
	//static void Callback(int other_arg, void * this_pointer);
	//static void otherCall();
	Objects * object[10];

	int ObjectId;

	std::vector<float>currentAnimSpeed;// = { 0,0,0,0 };
	std::vector<string>currentAnimName;// = { 0,0,0,0 };

	btVector3 min;
	btVector3 max;

	DirectX::XMFLOAT3 minTemp;
	DirectX::XMFLOAT3 maxTemp;

public:
	GunGameState();
	~GunGameState();

	void pause(bool paused);
	int getNrOfPlayers() const;
	Player* getPlayer(int id) const;

	bool initPlayers(AnimalType type[], PlayerColor color[]);
	bool initailize();
	bool render();
	void renderImgui();
	bool update(float deltaTime);
	void shutDown();
	void reset();
	bool controllerIsConnected(int controllerPort);

	bool checkPause() const;
	int getCameraFocus();
	bool checkCameraFocus();
	DirectX::XMFLOAT3 changeCamera(float deltaTime)const;

	DirectX::XMFLOAT3 rotateCamera(float deltaTime)const;
	/*static GunGameState * shelf;*/
	/*void callback(int other_arg, void * this_pointer);*/
	static bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2);
	//btCollisionObjectWrapper getGroundCollisionObject();

	//Temporary
	//box and box
	//bool collision(DirectX::XMFLOAT2 posOne, DirectX::XMFLOAT2 scaleOne,int playerID, DirectX::XMFLOAT2 posTwo, DirectX::XMFLOAT2 scaleTwo, int itemID);
	//circle and circle
	//bool collision(DirectX::XMFLOAT2 posOne, float radiusOne, DirectX::XMFLOAT2 posTwo, float radiusTwo);
	////box and circle
	//bool collision(DirectX::XMFLOAT2 posBox, DirectX::XMFLOAT2 scaleBox, DirectX::XMFLOAT2 posCircle, float radiusCircle);



};

#endif // !GUN_GAME_STATE_H
