#include "GunGameState.h"
#include "System.h"
#include "GunGameGui.h"
#include "PauseGui.h"
#include "ResultGui.h"

//GunGameState* GunGameState::shelf = nullptr;


//btRigidBody* GunGameState::addSphere(float rad, float x, float y, float z, float mass)
//{	//add object set transform
//	btTransform t; //
//	t.setIdentity();
//	t.setOrigin(btVector3(x, y,z));
//	set shape for object
//	btSphereShape * sphere = new btSphereShape(rad);
//
//	btVector3 inertia(0, 0, 0);
//	if (mass != 0, 0) {
//		sphere->calculateLocalInertia(mass, inertia);
//	}
//
//	set motionshape aka set postion
//	btMotionState* motion = new btDefaultMotionState(t);
//	body definition check doc
//	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);
//
//	btRigidBody* body = new btRigidBody(info);
//
//	world->addRigidBody(body);
//	bodies.push_back(body);
//	return body;
//}

//bool GunGameState::checkReset(DirectX::GamePad::State state)
//{
//
//	bool result = false;
//	//if (state.IsConnected())
//	//{
//		//tracker.Update(state);
//	if (((state.IsLeftTriggerPressed() && state.IsRightTriggerPressed()) ||
//		(state.buttons.leftShoulder && state.buttons.rightShoulder)) &&
//		state.buttons.a && (state.buttons.back || state.buttons.menu))
//	{
//		result = true;
//	}
//	//}
//	return result;
//}

//void GunGameState::Callback(int other_arg, void * this_pointer) 
//{
//	GunGameState * self = static_cast<GunGameState*>(this_pointer);
//	int lol = 0;
//	bool test = false;
//
//	test = self->checkPause();
//	//test = this->checkPause(); //this does not work!!
//	if (test == true)
//		lol = 500;
//	else
//		lol = 2;
//	shelf = self;
//}
//void GunGameState::otherCall() //jabbas exempel
//{
//	bool test = false; 
//	int lol = 0;
//	test = shelf->checkPause();
//	if (test == true)
//		lol = 500;
//	else
//		lol = 2;
//}

//Objects* GunGameState::object[];



GunGameState::GunGameState()
{
	this->testColBox = false;
	this->uiEnable = true;
	this->paused = false;
	this->resultsShown = false;
	this->inGameGui = nullptr;
	this->pauseGui = nullptr;
	this->resultGui = nullptr;
	this->cameraFocus = 0;
	this->objectId = 1;
	this->nrOfObjects = 0;
	this->capOfObjects = 10;
	this->nrOfRespawns = 0;
	this->capOfRespawns = 10;
	this->objects = new Objects*[this->capOfObjects];
	for (int i = 0; i < this->capOfObjects; i++)
	{
		this->objects[i] = nullptr;
	}

}

GunGameState::~GunGameState()
{
	shutDown();
}

bool GunGameState::callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{
	Player* PlrPointer = ((Player*)obj2->getCollisionObject()->getUserPointer());
	if (obj1->getCollisionObject()->getUserPointer() == (Player*)obj1->getCollisionObject()->getUserPointer())
	{
		if (((Player*)obj1->getCollisionObject()->getUserPointer()) != nullptr) {
			bool ishit = ((Player*)obj1->getCollisionObject()->getUserPointer())->getHit();
		}
		
		//wall ground
		Objects* pointer = (Objects*)obj1->getCollisionObject()->getUserPointer();
		switch (pointer->getId())
		{
		case 4:
			if (PlrPointer != nullptr && !PlrPointer->getHitStun()) {
				PlrPointer->setGrounded(true);
				if ((abs(pointer->getMovingSpeed().x) > 10 || pointer->getMovingSpeed().y < -5) && pointer->getCanGiveDmg()) {
					pointer->takeDmg(25);
					if (pointer->getHealth() <= 0) {
						System::getSoundManager()->playEffect("Stone_Getting_Destroyed");
					}
					PlrPointer->takeDamage(25);
					PlrPointer->setHitStun(true);

					System::getParticleManager()->addSimpleEffect(PlrPointer->getPosition(),"hit_effect",0.5f,2,false,30,6);

					int randomNumber = (rand() % 4) + 0;
					System::getSoundManager()->playEffect(to_string(randomNumber));

					int randomNumber2 = (rand() % 3) - 1;
					int randomNumber3 = (rand() % 3) - 1;
					int randomNumber4 = (rand() % 3) - 1;

					if (randomNumber2 == 0 && randomNumber3 == 0 && randomNumber4 == 0) {
						int randomNumber2 = (rand() % 3) - 1;
						int randomNumber3 = (rand() % 3) - 1;
						int randomNumber4 = (rand() % 3) - 1;
					}

					System::theCamera->cameraShake(0.1, DirectX::XMFLOAT3(PlrPointer->dir, randomNumber3, randomNumber4));

					if (PlrPointer->getHealth() <= 0){
						PlrPointer->setDiedOfStone(true);
						pointer->setPlayerKilled(true);
						System::getSoundManager()->playEffect("Death");
					}
				}
			}
			break;
		case 3:
			if (PlrPointer != nullptr) {
				PlrPointer->setGrounded(true);
				PlrPointer->addGroundMovingSpeed(pointer->getMovingSpeed());
			}
			break;
		case 2:
			if (PlrPointer != nullptr) {
				PlrPointer->setCanWallJump(true);
			}
			break;
		}
		//platform
		switch (pointer->GetType())
		{
		case DYNAMIC:
			//OutputDebugStringA("1\n");
			//if (((Player*)obj2->getCollisionObject()->getUserPointer()) != nullptr) {
			//	btTransform& trans = pointer->GetRigidBody()->getWorldTransform();
			////	((Player*)obj2->getCollisionObject()->getUserPointer())->follow(pointer->ObjectOBJ->GetPosition());
			//}
			break;
		}

	}
	if ((obj1->getCollisionObject()->getUserPointer() == (Objects*)obj1->getCollisionObject()->getUserPointer()) && ((Objects*)obj1->getCollisionObject()->getUserPointer() != nullptr)) {
		Objects* pointer = (Objects*)obj1->getCollisionObject()->getUserPointer();
		if (pointer != nullptr && pointer->GetState() == TRUE_DYNAMIC) {
			if ((abs(pointer->getMovingSpeed().x) > 5 || abs(pointer->getMovingSpeed().y) > 5)) {
				pointer->impactSoundEffect();
			}
		}
	}
	return false;
}

void GunGameState::pause(bool paused)
{
	this->paused = paused;

	if (paused)
	{
		this->pauseGui->activateDelay();
	}
}

int GunGameState::getNrOfPlayers() const
{
	return this->nrOfPlayers;
}

Player * GunGameState::getPlayer(int id) const
{
	return this->player[id];
}

void GunGameState::expandObjects()
{
	this->capOfObjects += 5;
	Objects* *temp = new Objects*[this->capOfObjects];
	for (int i = 0; i < this->nrOfObjects; i++)
	{
		temp[i] = this->objects[i];
	}
	delete[]this->objects;
	this->objects = temp;
}

void GunGameState::addObject(std::string filePath, btVector3 pos, int id, int friction, btVector3 size, btVector3 rotation, bool canBeDrawn, bool shouldRespawn)
{
	if (this->nrOfObjects == this->capOfObjects)
		this->expandObjects();
	this->objects[this->nrOfObjects] = new Objects(filePath, pos, id, friction, size,rotation, STATIC, STONE, -1, 1, 1, false, canBeDrawn);
	this->nrOfObjects++;
	
}

void GunGameState::addObject(std::string filePath, btVector3 pos, int id, int friction, btVector3 size, btVector3 rotation, OBJECTSTATE state, OBJECTYPE type, int mipLevels, bool shouldRespawn)
{

	if (this->nrOfObjects == this->capOfObjects)
		this->expandObjects();
	this->objects[this->nrOfObjects] = new Objects(filePath, pos, id, friction, size,rotation, state, type, mipLevels);

	this->nrOfObjects++;
}

void GunGameState::addObject(std::string filePath, btVector3 pos, int id, int friction, btVector3 size, btVector3 rotation, OBJECTSTATE state, OBJECTYPE type, bool shouldRespawn, int mipLevels, int xRepeated, int yRepeated, bool changeOpacity, bool canBeDrawn)
{
	if (this->nrOfObjects == this->capOfObjects)
		this->expandObjects();
	this->objects[this->nrOfObjects] = new Objects(filePath, pos, id, friction, size,rotation, state, type, mipLevels, xRepeated, yRepeated, changeOpacity, canBeDrawn);
	this->nrOfObjects++;
}

bool GunGameState::initPlayers(AnimalType type[], PlayerColor color[], bool rumbleEnabled[])
{
	for (int i = 0; i < nrOfPlayers; i++)
	{
		player[i]->setAnimalTypeAndMass(type[i]);
		player[i]->setColorMask(color[i]);
		player[i]->setRumble(rumbleEnabled[i]);
	}
	reset();
	return true;
}


bool GunGameState::initailize()
{
	gContactAddedCallback = callbackFunc;

	/*this->objects[0] = new Objects("Resources/Models/small_stone1.lu", btVector3(-10, 7, 0), 4, 2, btVector3(7.5f, 7.5f, 7.5f), TRUE_DYNAMIC, PLATFORM, 1);
	this->objects[1] = new Objects("Resources/Models/small_stone2.lu", btVector3(5, 6, 0), 4, 2, btVector3(7.5f, 7.5f, 7.5f), TRUE_DYNAMIC, PLATFORM, 1);
	this->objects[2] = new Objects("Resources/Models/small_stone3.lu", btVector3(12, 6, 0), 4, 2, btVector3(7.5f, 7.5f, 7.5f), TRUE_DYNAMIC, PLATFORM, 1);
	this->objects[3] = new Objects("Resources/Models/ground.lu", btVector3(16, 0, 20), 3, 3, btVector3(100.f, 4.f, 50.f), STATIC, STONE, -1, 10000, 10000,true);
	this->objects[4] = new Objects("Resources/Models/platform1.lu", btVector3(12, 4, 0), 3,3, btVector3(1.4f, 2.8f, 1.4f), DYNAMIC, PLATFORM,1);
	this->objects[5] = new Objects("Resources/Models/platform1.lu", btVector3(5,4, 0), 3,3, btVector3(1.4f, 2.8f, 1.4f),DYNAMIC, PLATFORM,1);
	this->objects[6] = new Objects("Resources/Models/cube2.lu", btVector3(35, 17, 0), 2,1, btVector3(10.f, 40.f, 10.f), STATIC,STONE,-1,1,1,false,false);
	this->objects[7] = new Objects("Resources/Models/cube2.lu", btVector3(-35, 17, 0), 2,1, btVector3(10.f, 40.f, 10.f), STATIC, STONE, -1, 1, 1, false, false);
	this->objects[8] = new Objects("Resources/Models/platform2.lu", btVector3(-10, 5, 0), 3, 3, btVector3(0.6f, 0.8f, 0.6f), STATIC, PLATFORM, 1);

	this->nrOfObjects = 9;*/

	this->addObject("Resources/Models/small_stone1.lu", btVector3(-10, 7, 0), 4, 2, btVector3(7.5f, 7.5f, 7.5f), btVector3(0.0, 0.0, 0.0), TRUE_DYNAMIC, PLATFORM, 1,true);
	this->addObject("Resources/Models/small_stone2.lu", btVector3(5, 6, 0), 4, 2, btVector3(7.5f, 7.5f, 7.5f), btVector3(0.0, 0.0, 0.0), TRUE_DYNAMIC, PLATFORM, 1,true);
	this->addObject("Resources/Models/small_stone3.lu", btVector3(12, 6, 0), 4, 2, btVector3(7.5f, 7.5f, 7.5f), btVector3(0.0, 0.0, 0.0), TRUE_DYNAMIC, PLATFORM, 1,true);

	this->addObject("Resources/Models/ground.lu", btVector3(16, 0, 20), 3, 3, btVector3(100.f, 4.f, 50.f), btVector3(0.0, 0.0, 0.0), STATIC, GROUND, false, -1, 10000, 10000, true);

	this->addObject("Resources/Models/platform1.lu", btVector3(12, 4, 0), 3, 3, btVector3(1.4f, 2.8f, 1.4f), btVector3(0.0, 0.0, 0.0), DYNAMIC, PLATFORM, 1,true);
	this->addObject("Resources/Models/platform1.lu", btVector3(5, 8, 0), 3, 3, btVector3(1.4f, 2.8f, 1.4f), btVector3(0.0, 0.0, 0.0), DYNAMIC, PLATFORM, 1,true);

	this->addObject("Resources/Models/cube2.lu", btVector3(35, 17, 0), 2, 1, btVector3(10.f, 40.f, 10.f), btVector3(0.0, 0.0, 0.0), STATIC, STONE,false, -1, 1, 1, false,false);
	this->addObject("Resources/Models/cube2.lu", btVector3(-35, 17, 0), 2, 1, btVector3(10.f, 40.f, 10.f), btVector3(0.0, 0.0, 0.0), STATIC, STONE,false, -1, 1, 1, false, false);

	this->addObject("Resources/Models/platform2.lu", btVector3(-17, 9, 0), 3, 3, btVector3(1.0f, 1.0f, 1.0f), btVector3(0.0, 0.0, 0.0), STATIC, PLATFORM, 1,true);
	this->addObject("Resources/Models/platform1.lu", btVector3(-3, 10, 0), 3, 3, btVector3(2.0f, 2.0f, 2.0f), btVector3(0.0, 0.0, 0.0), STATIC, PLATFORM, 1, true);

	this->addObject("Resources/Models/tree1.lu", btVector3(10, 0, 7.6), 0, 0, btVector3(1.6, 1.6, 1.6), btVector3(0.0, 0.0, 0.0), BACKGROUND, PLATFORM, 1, false);

	this->addObject("Resources/Models/bush1.lu", btVector3(-20, 1.5, -0.1), 0, 0, btVector3(2.6, 1.7, 1.7), btVector3(0.0, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/bush1.lu", btVector3(-23, 1.1, -0.1), 0, 0, btVector3(1.3, 1.1, 1.1), btVector3(0.0, 0.0, 0.0), BACKGROUND, STONE, 1, false);

	//GRASS
	this->addObject("Resources/Models/grass1.lu", btVector3( 0.0, 0.4, -1.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-1.0, 0.4, -1.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-2.0, 0.4, -1.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-3.0, 0.4, -1.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-4.0, 0.4, -2.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-5.0, 0.4, -2.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-6.0, 0.4, -2.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-7.0, 0.4, -1.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-8.0, 0.4, -1.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-9.0, 0.4, -1.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-10.0, 0.4, -1.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-11.0, 0.4, -1.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-12., 0.4, -2.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-13., 0.4, -2.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-14.0, 0.4, -1.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-15.0, 0.4, -0.6), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-16.0, 0.4, -1.7), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-17.0, 0.4, -1.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-18.0, 0.4, -2.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-19.0, 0.4, -2.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-20.0, 0.4, -1.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-21.0, 0.4, -1.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-22.0, 0.4, -1.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-23.0, 0.4, -1.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-24.0, 0.4, -1.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-25.0, 0.4, -2.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);

	this->addObject("Resources/Models/grass2.lu", btVector3(1.0, 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(2.0, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(3.0, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(4, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(5, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(6, 0.4, -0.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(7, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(8, 0.4, -0.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(9, 0.4, -0.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(10, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(11, 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(12, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(13., 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(14., 0.4, -0.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(15, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(16, 0.4, 0.6), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(17, 0.4, -0.7), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(18, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(19, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(20, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(21, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(22, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(23, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(24, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(25, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(26, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);

	this->addObject("Resources/Models/grass2.lu", btVector3( 0.4, 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-1.7, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-2.4, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-3.6, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-4.2, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-5.8, 0.4, -0.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-6.5, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-7.3, 0.4, -0.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-8.6, 0.4, -0.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-9.8, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-10.4, 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-11.7, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-12.2, 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-13.3, 0.4, -0.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-14.8, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-15.4, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-16.5, 0.4, -0.7), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-17.8, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-18.2, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-19.6, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-20.3, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-21.6, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-22.9, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-23.3, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-24.4, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-25.2, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);

	this->addObject("Resources/Models/grass2.lu", btVector3(1.5, 0.4, -1.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(2.5, 0.4, -1.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(3.5, 0.4, -1.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(4.5, 0.4, -2.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(5.5, 0.4, -1.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(6.5, 0.4, -2.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(7.5, 0.4, -1.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(8.5, 0.4, -2.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(9.5, 0.4, -1.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(10.5, 0.4, -2.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(11.5, 0.4, -1.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(12.5, 0.4, -2.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(13.5, 0.4, -1.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(14.5, 0.4, -2.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(15.5, 0.4, -2.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(16.5, 0.4, 1.6), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(17.5, 0.4, -1.7), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(18.5, 0.4, -1.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(19.5, 0.4, -1.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(20.5, 0.4, -2.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(21.5, 0.4, -2.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(22.5, 0.4, -1.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(23.5, 0.4, -2.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(24.5, 0.4, -1.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(25.5, 0.4, -1.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(26.5, 0.4, -2.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);

	//Foreground grass
	this->addObject("Resources/Models/grass2.lu", btVector3(0.4, 0.4, -8.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-1.7, 0.4, -8.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-2.4, 0.4, -8.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-3.6, 0.4, -8.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-4.2, 0.4, -8.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-5.8, 0.4, -8.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-6.5, 0.4, -8.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-7.3, 0.4, -8.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-8.6, 0.4, -8.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-9.8, 0.4, -8.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-10.4, 0.4, -8.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-11.7, 0.4, -8.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-12.2, 0.4, -8.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-13.3, 0.4, -8.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-14.8, 0.4, -8.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-15.4, 0.4, -8.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-16.5, 0.4, -8.7), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-17.8, 0.4, -8.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-18.2, 0.4, -8.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-19.6, 0.4, -8.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-20.3, 0.4, -8.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-21.6, 0.4, -8.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-22.9, 0.4, -8.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-23.3, 0.4, -8.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-24.4, 0.4, -8.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-25.2, 0.4, -8.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);

	this->addObject("Resources/Models/grass2.lu", btVector3(1.5, 0.4, -5.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(2.5, 0.4, -5.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(3.5, 0.4, -5.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(4.5, 0.4, -6.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(5.5, 0.4, -5.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(6.5, 0.4, -6.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(7.5, 0.4, -5.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(8.5, 0.4, -6.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(9.5, 0.4, -5.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(10.5, 0.4, -6.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(11.5, 0.4, -5.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(12.5, 0.4, -6.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(13.5, 0.4, -5.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(14.5, 0.4, -6.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(15.5, 0.4, -5.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(16.5, 0.4, 5.6), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(17.5, 0.4, -5.7), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(18.5, 0.4, -5.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(19.5, 0.4, -5.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(20.5, 0.4, -6.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(21.5, 0.4, -6.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(22.5, 0.4, -5.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(23.5, 0.4, -6.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(24.5, 0.4, -5.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(25.5, 0.4, -5.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(26.5, 0.4, -6.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(20, 0.0, 0.0), BACKGROUND, STONE, 1, false);

	this->addObject("Resources/Models/grass1.lu", btVector3(0.0, 0.4, -10.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-1.0, 0.4, -10.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-2.0, 0.4, -10.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-3.0, 0.4, -10.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-4.0, 0.4, -11.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-5.0, 0.4, -11.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-6.0, 0.4, -11.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-7.0, 0.4, -10.5), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-8.0, 0.4, -10.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-9.0, 0.4, -10.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-10.0, 0.4, -10.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-11.0, 0.4, -10.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-12., 0.4, -11.4), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-13., 0.4, -11.1), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-14.0, 0.4, -10.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-15.0, 0.4, -9.6), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-16.0, 0.4, -10.7), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-17.0, 0.4, -10.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-18.0, 0.4, -11.2), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-19.0, 0.4, -11.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-20.0, 0.4, -10.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-21.0, 0.4, -10.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-22.0, 0.4, -10.0), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-23.0, 0.4, -10.3), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-24.0, 0.4, -10.8), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-25.0, 0.4, -11.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);

	this->addObject("Resources/Models/grass2.lu", btVector3(0, 0.4, 4.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-2, 0.4, 3.9), 0, 0, btVector3(0.9, 0.9, 0.9), btVector3(0.0, 0.0, 0.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass_patch1.lu", btVector3(0, 0.4, 4.9), 0, 0, btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), BACKGROUND, STONE, 1, false);


	//Background trees
	GameObject* tree2 = new GameObject;
	System::theModelLoader->loadGO(tree2, "Resources/Models/tree2.lu");
	System::handler->addObject(tree2);
	tree2->setPosition(-1, 1, 26);
	tree2->setScale(0.7, 0.7, 0.7);
	tree2->setRotationRollPitchYaw(0, -0.5, 0);
	GameObject* tree3 = new GameObject;
	System::theModelLoader->loadGO(tree3, "Resources/Models/tree3.lu");
	System::handler->addObject(tree3);
	tree3->setPosition(-40, 0, 25);
	tree3->setScale(0.8, 0.8, 0.8);
	tree3->setRotationRollPitchYaw(0, -2.5, 0);
	GameObject* tree4 = new GameObject;
	System::theModelLoader->loadGO(tree4, "Resources/Models/tree3.lu");
	System::handler->addObject(tree4);
	tree4->setPosition(45, 0, 40);
	tree4->setScale(0.6, 0.6, 0.6);
	tree4->setRotationRollPitchYaw(0, 2, 0);

	//GameObject* obj = new GameObject;
	//System::theModelLoader->loadGO(obj, "Resources/Models/platform2.lu");
	//System::handler->addObject(obj);
	//obj->setPosition(-12, 2, 15);
	////obj->setScale(0.6, 0.6, 0.6);
	//obj->setRotationRollPitchYaw(2.4, 2.3, 2.5);

	//Grass
	GameObject* grass_patch1 = new GameObject;
	System::theModelLoader->loadGO(grass_patch1, "Resources/Models/grass_patch1.lu");
	System::handler->addObject(grass_patch1);
	grass_patch1->setPosition(0, 0.8, -5.9);
	GameObject* grass_patch2 = new GameObject;
	System::theModelLoader->loadGO(grass_patch2, "Resources/Models/grass_patch1.lu");
	System::handler->addObject(grass_patch2);
	grass_patch2->setPosition(0, 0.8, 12);
	GameObject* grass_patch3 = new GameObject;
	System::theModelLoader->loadGO(grass_patch3, "Resources/Models/grass_patch1.lu");
	System::handler->addObject(grass_patch3);
	grass_patch3->setPosition(8, 0.8, 9);
	GameObject* grass_patch4 = new GameObject;
	System::theModelLoader->loadGO(grass_patch4, "Resources/Models/grass_patch1.lu");
	System::handler->addObject(grass_patch4);
	grass_patch4->setPosition(-20, 0.8, 5);
	GameObject* grass_patch5 = new GameObject;
	System::theModelLoader->loadGO(grass_patch5, "Resources/Models/grass_patch1.lu");
	System::handler->addObject(grass_patch5);
	grass_patch5->setPosition(-20, 0.8, 17);

	//Background stones
	GameObject* cliff1 = new GameObject;
	System::theModelLoader->loadGO(cliff1, "Resources/Models/cliff1.lu");
	System::handler->addObject(cliff1);
	cliff1->setPosition(35, 10, 7.6);
	cliff1->setScale(10, 10, 10);
	GameObject* cliff2 = new GameObject;
	System::theModelLoader->loadGO(cliff2, "Resources/Models/cliff2.lu");
	System::handler->addObject(cliff2);
	cliff2->setPosition(20, 0, 7.6);
	cliff2->setScale(9.8, 9.8, 9.8);
	GameObject* cliff3 = new GameObject;
	System::theModelLoader->loadGO(cliff3, "Resources/Models/cliff3.lu");
	System::handler->addObject(cliff3);
	cliff3->setPosition(20, 10, 27.6);
	cliff3->setScale(9.8, 9.8, 9.8);
	GameObject* cliff4 = new GameObject;
	System::theModelLoader->loadGO(cliff4, "Resources/Models/cliff1.lu");
	System::handler->addObject(cliff4);
	cliff4->setPosition(-37, 5.6, 9.6);
	cliff4->setScale(5, 5, 5);
	cliff4->setRotationRollPitchYaw(0, -2.5, 0);
	GameObject* cliff5 = new GameObject;
	System::theModelLoader->loadGO(cliff5, "Resources/Models/cliff4.lu");
	System::handler->addObject(cliff5);
	cliff5->setPosition(33, 0.6, 30);
	cliff5->setScale(5, 5, 5);
	cliff5->setRotationRollPitchYaw(0, -2.5, 0);
	GameObject* cliff6 = new GameObject;
	System::theModelLoader->loadGO(cliff6, "Resources/Models/cliff2.lu");
	System::handler->addObject(cliff6);
	cliff6->setPosition(-20, 0, 27.6);
	cliff6->setScale(9.8, 9.8, 9.8);
	GameObject* cliff7 = new GameObject;
	System::theModelLoader->loadGO(cliff7, "Resources/Models/cliff2.lu");
	System::handler->addObject(cliff7);
	cliff7->setPosition(-23, 3, 20.6);
	cliff7->setScale(3.8, 2.8, 3.8);

	//Mushrooms
	GameObject* mushrooms1 = new GameObject;
	System::theModelLoader->loadGO(mushrooms1, "Resources/Models/mushroom_group.lu");
	System::handler->addObject(mushrooms1);
	mushrooms1->setPosition(7, 0.9, 6.0);
	GameObject* mushroom1 = new GameObject;
	System::theModelLoader->loadGO(mushroom1, "Resources/Models/mushroom1.lu");
	System::handler->addObject(mushroom1);
	mushroom1->setPosition(2.5, 0.9, 18.0);
	mushroom1->setScale(2, 2, 2);
	GameObject* mushroom2 = new GameObject;
	System::theModelLoader->loadGO(mushroom2, "Resources/Models/mushroom1.lu");
	System::handler->addObject(mushroom2);
	mushroom2->setPosition(-18, 0.9, -2.0);
	mushroom2->setScale(1.3, 1.3, 1.3);
	GameObject* mushroom3 = new GameObject;
	System::theModelLoader->loadGO(mushroom3, "Resources/Models/mushroom2.lu");
	System::handler->addObject(mushroom3);
	mushroom3->setPosition(15, 0.9, 4.0);
	mushroom3->setScale(1.3, 1.3, 1.3);
	GameObject* mushroom4 = new GameObject;
	System::theModelLoader->loadGO(mushroom4, "Resources/Models/mushroom2.lu");
	System::handler->addObject(mushroom4);
	mushroom4->setPosition(2, 0.9, 25.0);
	GameObject* mushroom5 = new GameObject;
	System::theModelLoader->loadGO(mushroom5, "Resources/Models/mushroom2.lu");
	System::handler->addObject(mushroom5);
	mushroom5->setPosition(0, 0.9, 21.0);
	mushroom3->setScale(0.8, 0.8, 0.8);
	GameObject* mushroom6 = new GameObject;
	System::theModelLoader->loadGO(mushroom6, "Resources/Models/mushroom3.lu");
	System::handler->addObject(mushroom6);
	mushroom6->setPosition(-15, 0.9, 5.0);
	mushroom6->setScale(3, 3, 3);
	GameObject* mushroom7 = new GameObject;
	System::theModelLoader->loadGO(mushroom7, "Resources/Models/mushroom3.lu");
	System::handler->addObject(mushroom7);
	mushroom7->setPosition(-17, 0.9, 4.0);
	mushroom7->setScale(2, 2, 2);
	GameObject* mushroom8 = new GameObject;
	System::theModelLoader->loadGO(mushroom8, "Resources/Models/mushroom3.lu");
	System::handler->addObject(mushroom8);
	mushroom8->setPosition(-14, 0.9, 8.0);
	mushroom8->setScale(0.8, 0.8, 0.8);
	GameObject* mushroom9 = new GameObject;
	System::theModelLoader->loadGO(mushroom9, "Resources/Models/mushroom3.lu");
	System::handler->addObject(mushroom9);
	mushroom9->setPosition(-20, 0.9, 8.1);
	mushroom9->setScale(0.8, 0.8, 0.8);

	//Bushes
	GameObject* bush1 = new GameObject;
	System::theModelLoader->loadGO(bush1, "Resources/Models/bush1.lu");
	System::handler->addObject(bush1);
	bush1->setPosition(24, 2.5, -1.0);
	bush1->setScale(3, 3, 3);
	bush1->setRotationRollPitchYaw(0, -2.0, 0);
	GameObject* bush2 = new GameObject;
	System::theModelLoader->loadGO(bush2, "Resources/Models/bush1.lu");
	System::handler->addObject(bush2);
	bush2->setPosition(-4, 3.0, 20.0);
	bush2->setScale(3.5, 2.5, 2.5);
	bush2->setRotationRollPitchYaw(0, -2.5, 0);
	GameObject* bush3 = new GameObject;
	System::theModelLoader->loadGO(bush3, "Resources/Models/bush1.lu");
	System::handler->addObject(bush3);
	bush3->setPosition(-30, 2.5, -6.0);
	bush3->setScale(4, 3, 3);
	bush3->setRotationRollPitchYaw(0, -1.5, 0);
	GameObject* bush4 = new GameObject;
	System::theModelLoader->loadGO(bush4, "Resources/Models/bush1.lu");
	System::handler->addObject(bush4);
	bush4->setPosition(-17, 2.5, 22.0);
	bush4->setScale(4, 2, 2);
	bush4->setRotationRollPitchYaw(0, -0.5, 0);

	//Trees at the border of the stage
	GameObject* rBorderTree1 = new GameObject;
	System::theModelLoader->loadGO(rBorderTree1, "Resources/Models/tree4.lu");
	System::handler->addObject(rBorderTree1);
	rBorderTree1->setPosition(30, 0.5, -2);
	rBorderTree1->setScale(0.7, 0.7, 0.7);
	GameObject* rBorderTree2 = new GameObject;
	System::theModelLoader->loadGO(rBorderTree2, "Resources/Models/tree1.lu");
	System::handler->addObject(rBorderTree2);
	rBorderTree2->setPosition(31, 0, 2.6);
	rBorderTree2->setScale(1.6, 1.6, 1.6);
	rBorderTree2->setRotationRollPitchYaw(0, -0.8, 0);
	GameObject* lBorderTree1 = new GameObject;
	System::theModelLoader->loadGO(lBorderTree1, "Resources/Models/tree4.lu");
	System::handler->addObject(lBorderTree1);
	lBorderTree1->setPosition(-31.7, 0.5, -2);
	lBorderTree1->setScale(1.2, 1.2, 1.2);

	//Misc
	GameObject* ray1 = new GameObject;
	System::theModelLoader->loadGO(ray1, "Resources/Models/sun_ray1.lu");
	System::handler->addObject(ray1);
	ray1->setPosition(0, 30, 0);
	ray1->setScale(1.5, 1.5, 1.5);
	ray1->setRotationRollPitchYaw(0, 0, -0.5);
	GameObject* ray2 = new GameObject;
	System::theModelLoader->loadGO(ray2, "Resources/Models/sun_ray1.lu");
	System::handler->addObject(ray2);
	ray2->setPosition(15, 30, 0);
	ray2->setScale(1.5, 1.5, 1.5);
	ray2->setRotationRollPitchYaw(0, 0, -0.5);

	this->nrOfPlayers = 4;
	this->currentAnimSpeed.resize(this->nrOfPlayers);
	this->currentAnimName.resize(this->nrOfPlayers);
	this->currentAnimLoop.resize(this->nrOfPlayers);
	player = new Player*[nrOfPlayers];
	this->spawnPoints[0] = btVector3(5, 3, 0);
	this->spawnPoints[1] = btVector3(-5, 3, 0);
	this->spawnPoints[2] = btVector3(15, 3, 0);
	this->spawnPoints[3] = btVector3(-15, 3, 0);
	for (int i = 0; i < nrOfPlayers; i++)
	{
		player[i] = new Player();
		player[i]->initialize(FOX, RED);
		player[i]->setRigidbodyPosition(this->spawnPoints[i].getX(), this->spawnPoints[i].getY(), this->spawnPoints[i].getZ());
		this->player[i]->setDirection(-1);//everyone should look towards the middle
		
	}
	//this->player[2]->playerObj->setRotationRollPitchYaw(0.f, 1.5*(3.14f), 0.f);

	//player[1]->setRigidbodyPosition(-10,10, 0.f);
	System::handler->initialize();
	System::handler->setSkyboxTexture("painted_skybox2");
	float pos[4] = {
	0.f ,0.3f ,0.f,10.f
	};
	float dir[4] = {
		0.1f,-1.0f,0.0f,1.0f
	};
	float color[4] = {
		1.0f , 1.0f, 0.8f , 1.4f
	};
	System::handler->addLight(pos, dir, color);
	float color2[4] = {
		0.5f , 0.0f, 1.0f , 1.0f
	};
	System::handler->addLight(pos, dir, color2);
	color2[0] = 0.0f;
	color2[1] = 1.0f;
	color2[2] = 1.0f;
	pos[0] = -10.0f;
	pos[1] = 4;
	pos[2] = -2;
	color2[3] = 1.0f;
	System::handler->addLight(pos, dir, color2);


	//Cool lights
	float color3[4] = {
		0.5f , 1.0f, 0.0f , 2.0f
	};
	float pos2[4] = {
	-18.f ,2.6f ,0.f,5.f
	};
	System::handler->addLight(pos2, dir, color3);

	float color4[4] = {
		1.0f , 0.0f, 0.5f , 15.0f
	};
	float pos3[4] = {
	10.f ,5.6f ,0.f,5.f
	};
	System::handler->addLight(pos3, dir, color4);

	float color5[4] = {
		0.0f , 0.5f, 1.0f , 25.0f
	};
	float pos4[4] = {
	-23.f ,7.6f ,0.f,5.f
	};
	System::handler->addLight(pos4, dir, color5);

	this->inGameGui = new GunGameGui(this);
	this->inGameGui->initialize();
	this->pauseGui = new PauseGui(this);
	this->pauseGui->initialize();
	this->resultGui = new ResultGui(this);
	this->resultGui->initialize();

	System::handler->sortBackToFront();


	// & to a new vector that holds the dynmic objects for collisons
	for (int i = 0; i < nrOfObjects; i++)
	{
		if (this->objects[i]->GetType() == GRASS) {
			this->dynamicObjects.push_back(objects[i]);
		}
		else if (this->objects[i]->GetType() == TRUE_DYNAMIC)
		{
			this->dynamicObjects.push_back(objects[i]);
		}
	}


	return true;
}

bool GunGameState::render()
{
	for (int pSpd = 0; pSpd < nrOfPlayers; pSpd++)
	{
		currentAnimSpeed[pSpd]= player[pSpd]->getAnimSpeed();

	}
	for (int pNm = 0; pNm < nrOfPlayers; pNm++)
	{
		currentAnimName[pNm] = player[pNm]->getAnimName();

	}
	for (int pLp = 0; pLp < nrOfPlayers; pLp++)
	{
		currentAnimLoop[pLp] = player[pLp]->getAnimLoop();

	}


	System::shaderManager->getParticleShader()->setViewProj(System::theCamera->GetViewMatrix(), System::theGraphicDevice->getProj(), DirectX::XMFLOAT4(System::theCamera->GetPosition().x, System::theCamera->GetPosition().y, System::theCamera->GetPosition().z, 1.0f));

	System::handler->draw(System::fusk->getDeltaTime(), this->paused, currentAnimSpeed, currentAnimName, currentAnimLoop);

	

	System::fusk->resetShaders();

	if (this->resultsShown)
	{
		this->resultGui->render();
	}
	else
	{
		if (this->uiEnable)
		{
			this->inGameGui->render();

			if (this->paused)
			{
				this->pauseGui->render();
			}
		}
	}
	renderImgui();

	return true;
}

void GunGameState::renderImgui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	std::string textUse;
	ImGui::Begin("Gungame");
	ImGui::Text("Hold 'V' To move camera with mouse.");
	btVector3 pos = this->player[0]->playerObj->getRigidbody()->getWorldTransform().getOrigin();

	std::string playerPos = "Player pos: " + std::to_string(pos.getX()) + " " +
		std::to_string(pos.getY()) + " " +
		std::to_string(pos.getZ());
	ImGui::Text(playerPos.c_str());

	if (this->testColBox)
	{
		ImGui::Text("Kolliderar ja");
	}
	else
	{
		ImGui::Text("Kolliderar nej");
	}
	ImGui::CaptureKeyboardFromApp(true);
	ImGui::Checkbox("Debug Draw",&System::getDebugDraw()->DebugDraw);
	ImGui::Checkbox("UI disable", &this->uiEnable);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Controllers");
	ImGui::BeginChild("Scrolling");
	
	for (int n = 0; n < 4; n++)
	{
		if (controllerIsConnected(n) == true)
			ImGui::Text("%02d: Connected", n);
		else
			ImGui::Text("%02d: Disconnected", n);
	}
	ImGui::EndChild();
	ImGui::End();
}

bool GunGameState::update(float deltaTime)
{

	

	if (resultsShown)
	{
		this->resultGui->update(deltaTime);
		
		if (System::getCurrentState() != this)
		{
			reset();
		}

		return true;
	}

	if (paused)
	{
		this->pauseGui->update(deltaTime);
		return true;
	}

	for (int i = 0; i < this->nrOfObjects; i++)
	{
		this->objects[i]->update(deltaTime);
	}

	for (int i = 0; i < nrOfPlayers; i++)
	{
		player[i]->playerObj->getRigidbody()->getAabb(min, max);
		minTemp = DirectX::XMFLOAT3(min.getX(), min.getY(), min.getZ());
		maxTemp = DirectX::XMFLOAT3(max.getX(), max.getY(), max.getZ());
		for (int j = 0; j < nrOfPlayers; j++) 
		{
			//Collision with Player Hitbox
			if (i != j)
			{
				if (Intersects(minTemp, maxTemp, player[j]->hitbox.hitbox->getCollisionBox(), player[j]->hitbox.hitbox->getPosition()) && !player[i]->getHitStun())
				{
					player[i]->setHitStun(true);
					this->testColBox = true;
					player[i]->playerObj->getRigidbody()->applyCentralImpulse(btVector3(player[j]->dir * 150 * ((player[j]->getWeight() + 2) /3), 150 * ((player[j]->getWeight() + 2) / 3), 0));// , btVector3(1, 0, 0));
					
					int tempHP = player[i]->getHealth();

					//TAKE DAMAGE HERE
					player[i]->takeDamage(player[j]->getStrength());
					player[i]->stats.damageTaken += player[j]->getStrength();
					player[j]->stats.damageDealt += player[j]->getStrength();
					System::getParticleManager()->addSimpleEffect(player[i]->getPosition(), "hit_effect", 0.5f, 2,false, 30, 6);

					int randomNumber = (rand() % 4) + 0;
					System::getSoundManager()->playEffect(to_string(randomNumber));

					int randomNumber2 = (rand() % 3) - 1;
					int randomNumber3 = (rand() % 3) - 1;
					int randomNumber4 = (rand() % 3) - 1;

					if (randomNumber2 == 0 && randomNumber3 == 0 && randomNumber4 == 0) {
						int randomNumber2 = (rand() % 3) - 1;
						int randomNumber3 = (rand() % 3) - 1;
						int randomNumber4 = (rand() % 3) - 1;
					}

					System::theCamera->cameraShake(0.1,DirectX::XMFLOAT3(player[j]->dir, randomNumber3, randomNumber4));

					if(player[i]->getHealth() <= 0 && tempHP > 0) {
						
						player[i]->stats.deaths++;
						player[j]->stats.kills++;

						if(player[j]->canChange()){
							player[j]->changeCharacter();
							System::getSoundManager()->playEffect("Death");
						}
						else
						{
							this->resultsShown = true;
							static_cast<ResultGui*>(this->resultGui)->initializePlayerStats();

							this->paused = true;
							this->pauseGui->activateDelay();
						}
					}

					//player[i]->setGrounded(true);
				}
				else
				{
					this->testColBox = false;
				}
				

			}
		}
		//Collision with Object Hitbox
		for (int i = 0; i < 6; i++) {
			if (objects[i]->GetState() == TRUE_DYNAMIC) {
				
				btVector3 minObj;
				btVector3 maxObj;

				DirectX::XMFLOAT3 minTempObj;
				DirectX::XMFLOAT3 maxTempObj;
				objects[i]->ObjectOBJ->getRigidbody()->getAabb(minObj, maxObj);
				minTempObj = DirectX::XMFLOAT3(minObj.getX(), minObj.getY(), minObj.getZ());
				maxTempObj = DirectX::XMFLOAT3(maxObj.getX(), maxObj.getY(), maxObj.getZ());
				for (int j = 0; j < nrOfPlayers; j++) {
					if (Intersects(minTempObj, maxTempObj, player[j]->hitbox.hitbox->getCollisionBox(), player[j]->hitbox.hitbox->getPosition()) && !player[j]->getHitStun()) 
					{
						objects[i]->addImpulse(player[j]->dir * 65 * ((player[j]->getWeight()+1)/2), j);
						objects[i]->setLastPlayerHit(j);
					}
					if (objects[i]->getPlayerKilled() == true && player[j]->getDiedOfStone() == true) 
					{
						if (player[objects[i]->getLastPlayerHit()] != nullptr)
						{ 
							player[objects[i]->getLastPlayerHit()]->changeCharacter();
							objects[i]->setPlayerKilled(false);
							player[j]->setDiedOfStone(false);
						}
						else 
						{
							objects[i]->setPlayerKilled(false);
							player[j]->setDiedOfStone(false);
						}
					}
				}
			}
		}
		//GRASS ROTATION
		for (int i = 13; i < 115; i++)
		{
			for (int j = 0; j < nrOfPlayers; j++) {
				if (objects[i]->GetType() == GRASS) {
					if ((((objects[i]->getPosition().x - player[j]->getPosition().x) < 2) && ((objects[i]->getPosition().x - player[j]->getPosition().x) > -2)) && player[j]->getPosition().y < 2) {
						objects[i]->addGrassRotation(0.005, player[j]->dir);
						break;
					}
				}
			}
			for (int j = 0; j < 6; j++) {
				if (objects[i]->GetType() == GRASS && objects[j]->GetState() == TRUE_DYNAMIC) {
					if ((((objects[i]->getPosition().x - objects[j]->getRigidBodyPosition().getX()) < 1) && ((objects[i]->getPosition().x - objects[j]->getRigidBodyPosition().getX()) > -1)) && objects[j]->getRigidBodyPosition().getY() < 2) {
						objects[i]->addGrassRotation(0.01, objects[j]->getMovingDirection());
						break;
					}
				}
			}
		}


		////GRASS ROTATION
		//for (int i = 0; i < dynamicObjects.size(); i++) {
		//	for (int j = 0; j < nrOfPlayers; j++) {
		//		if (dynamicObjects[i]->GetType() == GRASS) {
		//			if ((((dynamicObjects[i]->getPosition().x - player[j]->getPosition().x) < 2) && ((dynamicObjects[i]->getPosition().x - player[j]->getPosition().x) > -2)) && player[j]->getPosition().y < 2) {
		//				dynamicObjects[i]->addGrassRotation(0.005, player[j]->dir);
		//			}
		//		}
		//	}
		//	for (int j = 0; j < dynamicObjects.size(); j++) {
		//		if (dynamicObjects[i]->GetType() == GRASS && dynamicObjects[j]->GetState() == TRUE_DYNAMIC) {
		//			if ((((dynamicObjects[i]->getPosition().x - dynamicObjects[j]->getRigidBodyPosition().getX()) < 1) && ((dynamicObjects[i]->getPosition().x - dynamicObjects[j]->getRigidBodyPosition().getX()) > -1)) && dynamicObjects[j]->getRigidBodyPosition().getY() < 2) {
		//				dynamicObjects[i]->addGrassRotation(0.01, dynamicObjects[j]->getMovingDirection());
		//			}
		//		}
		//	}
		//}

		//for(int k=0;k<nrOfObjects;k++){}

		//if(Intersect())



		player[i]->update(deltaTime, i);
		player[i]->updateRumble(deltaTime, i);

		//if (/*System::getphysices()->getPlaneRigidBody()->getPlaneConstant()*/  max.getY() < 1.2f){
		////	//DirectX::XMFLOAT3 aabbmin, DirectX::XMFLOAT3 aabbMax,const AABB b, XMFLOAT3 posB
		//	player[i]->setGrounded(true);
		//}
	}
	//System::getphysices()->getPlaneRigidBody()->getpl().getY();
	//if (Intersects(System::handler->getObject(2).getCollisionBox(), System::handler->getObject(2).getPosition(), System::handler->getObject(3).getCollisionBox(), System::handler->getObject(3).getPosition()))
	//{

	//}

	this->inGameGui->update(deltaTime);
	System::getParticleManager()->update(deltaTime);
	return true;
}

void GunGameState::shutDown()
{
	/*delete dispatcher;
	delete collisionConfig;
	delete solver;
	delete broadphase;
	delete world;
*/
	for (int i = 0; i < this->nrOfObjects; i++)
	{
		delete this->objects[i];
	}
	delete[] this->objects;
	for (int i = 0; i < nrOfPlayers; i++)
	{
		delete player[i];
	}
	delete[] player;

	delete this->inGameGui;
	delete this->pauseGui;
	delete this->resultGui;
	this->inGameGui = nullptr;
	this->pauseGui = nullptr;
	this->resultGui = nullptr;
}

void GunGameState::reset()
{
	for (int i = 0; i < nrOfPlayers; i++)
	{
		this->player[i]->reset();
		this->player[i]->setRigidbodyPosition(this->spawnPoints[i].getX(), this->spawnPoints[i].getY(), this->spawnPoints[i].getZ());
	}
	for (int i = 0; i < this->nrOfObjects; i++)
	{
		this->objects[i]->respawn(0);
	}

	this->paused = false;
	this->resultsShown = false;

	//Musica
	//System::getSoundManager()->playLooped(L"Battle_Theme_One_1.0.wav");
}

bool GunGameState::controllerIsConnected(int controllerPort)
{
	return System::theGamePad->GetState(controllerPort).IsConnected();
}

bool GunGameState::checkPause() const
{
	return this->paused;
}

int GunGameState::getCameraFocus() //focus on players
{
	if (this->cameraFocus < 0)
		this->cameraFocus = nrOfPlayers-1;
	else if (this->cameraFocus > nrOfPlayers - 1)
		this->cameraFocus = 0;
	return this->cameraFocus;
}

bool GunGameState::checkCameraFocus()
{
	
	bool result = false;
	DirectX::GamePad::State state = System::theGamePad->GetState(0);
//	System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED
	if (System::theTracker->leftTrigger == DirectX::GamePad::ButtonStateTracker::PRESSED ||
		System::theTracker->leftShoulder == DirectX::GamePad::ButtonStateTracker::PRESSED)
	{
		this->cameraFocus--;
		result = true;
	}
	else if(System::theTracker->rightTrigger == DirectX::GamePad::ButtonStateTracker::PRESSED || 
			System::theTracker->rightShoulder == DirectX::GamePad::ButtonStateTracker::PRESSED)
	{
		this->cameraFocus++;
		result = true;
	}
	

	return result;
}

DirectX::XMFLOAT3 GunGameState::changeCamera(float deltaTime)const
{
	//during pause
	DirectX::XMFLOAT3 camera = {0,0,0};

	DirectX::GamePad::State state = System::theGamePad->GetState(0);
	float dirX = 0.2f * state.thumbSticks.leftX * deltaTime;
	float dirY = 8.f * state.thumbSticks.leftY * deltaTime;
	float zoom = 0.3f * (state.buttons.y - state.buttons.x)*deltaTime;
	camera.x += dirX;
	camera.y += dirY;
	camera.z += zoom;
	return camera;
}

DirectX::XMFLOAT3 GunGameState::rotateCamera(float deltaTime) const
{
	DirectX::XMFLOAT3 rotation = { 0,0,0 };
	DirectX::GamePad::State state = System::theGamePad->GetState(0);
	float rotX = 35.f * state.thumbSticks.rightX*deltaTime;
	float rotY = 35.f * state.thumbSticks.rightY*deltaTime;
	rotation.x += -rotY;
	rotation.y += rotX;
	//rotation.x
	return rotation;
}
