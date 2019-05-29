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
				if ((pointer->getMovingSpeed().x > 20 || pointer->getMovingSpeed().y > 4) && pointer->getCanGiveDmg()) {
					pointer->takeDmg(25);
					PlrPointer->takeDamage(25);
					PlrPointer->setHitStun(true);

					System::getParticleManager()->addSimpleEffect(PlrPointer->getPosition(),"splat",1.0f);

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

void GunGameState::addObject(std::string filePath, btVector3 pos, int id, int friction, btVector3 size, bool canBeDrawn, bool shouldRespawn)
{
	if (this->nrOfObjects == this->capOfObjects)
		this->expandObjects();
	this->objects[this->nrOfObjects] = new Objects(filePath, pos, id, friction, size, STATIC, STONE, -1, 1, 1, false, canBeDrawn);
	this->nrOfObjects++;
	
}

void GunGameState::addObject(std::string filePath, btVector3 pos, int id, int friction, btVector3 size, OBJECTSTATE state, OBJECTYPE type, int mipLevels, bool shouldRespawn)
{

	if (this->nrOfObjects == this->capOfObjects)
		this->expandObjects();
	this->objects[this->nrOfObjects] = new Objects(filePath, pos, id, friction, size, state, type, mipLevels);

	this->nrOfObjects++;
}

void GunGameState::addObject(std::string filePath, btVector3 pos, int id, int friction, btVector3 size, OBJECTSTATE state, OBJECTYPE type, bool shouldRespawn, int mipLevels, int xRepeated, int yRepeated, bool changeOpacity, bool canBeDrawn)
{
	if (this->nrOfObjects == this->capOfObjects)
		this->expandObjects();
	this->objects[this->nrOfObjects] = new Objects(filePath, pos, id, friction, size, state, type, mipLevels, xRepeated, yRepeated, changeOpacity, canBeDrawn);
	this->nrOfObjects++;
}

bool GunGameState::initPlayers(AnimalType type[], PlayerColor color[])
{
	for (int i = 0; i < nrOfPlayers; i++)
	{
		player[i]->setAnimalTypeAndMass(type[i]);
		player[i]->setColorMask(color[i]);
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

	this->addObject("Resources/Models/small_stone1.lu", btVector3(-10, 7, 0), 4, 2, btVector3(7.5f, 7.5f, 7.5f), TRUE_DYNAMIC, PLATFORM, 1,true);
	this->addObject("Resources/Models/small_stone2.lu", btVector3(5, 6, 0), 4, 2, btVector3(7.5f, 7.5f, 7.5f), TRUE_DYNAMIC, PLATFORM, 1,true);
	this->addObject("Resources/Models/small_stone3.lu", btVector3(12, 6, 0), 4, 2, btVector3(7.5f, 7.5f, 7.5f), TRUE_DYNAMIC, PLATFORM, 1,true);

	this->addObject("Resources/Models/ground.lu", btVector3(16, 0, 20), 3, 3, btVector3(100.f, 4.f, 50.f), STATIC, GROUND, false, -1, 10000, 10000, true);

	this->addObject("Resources/Models/platform1.lu", btVector3(12, 4, 0), 3, 3, btVector3(1.4f, 2.8f, 1.4f), DYNAMIC, PLATFORM, 1,true);
	this->addObject("Resources/Models/platform1.lu", btVector3(5, 4, 0), 3, 3, btVector3(1.4f, 2.8f, 1.4f), DYNAMIC, PLATFORM, 1,true);

	this->addObject("Resources/Models/cube2.lu", btVector3(35, 17, 0), 2, 1, btVector3(10.f, 40.f, 10.f), STATIC, STONE,false, -1, 1, 1, false,false);
	this->addObject("Resources/Models/cube2.lu", btVector3(-35, 17, 0), 2, 1, btVector3(10.f, 40.f, 10.f), STATIC, STONE,false, -1, 1, 1, false, false);

	this->addObject("Resources/Models/platform2.lu", btVector3(-10, 5, 0), 3, 3, btVector3(0.6f, 0.8f, 0.6f), STATIC, PLATFORM, 1,true);

	this->addObject("Resources/Models/tree1.lu", btVector3(10, 0, 7.6), 0, 0, btVector3(1.6, 1.6, 1.6), BACKGROUND, PLATFORM, 1, false);

	//GRASS
	this->addObject("Resources/Models/grass1.lu", btVector3( 0.0, 0.4, -1.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-1.0, 0.4, -1.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-2.0, 0.4, -1.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-3.0, 0.4, -1.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-4.0, 0.4, -2.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-5.0, 0.4, -2.5), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-6.0, 0.4, -2.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-7.0, 0.4, -1.5), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-8.0, 0.4, -1.1), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-9.0, 0.4, -1.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-10.0, 0.4, -1.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-11.0, 0.4, -1.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-12., 0.4, -2.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-13., 0.4, -2.1), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-14.0, 0.4, -1.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-15.0, 0.4, -0.6), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-16.0, 0.4, -1.7), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-17.0, 0.4, -1.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-18.0, 0.4, -2.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-19.0, 0.4, -2.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-20.0, 0.4, -1.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-21.0, 0.4, -1.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-22.0, 0.4, -1.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-23.0, 0.4, -1.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-24.0, 0.4, -1.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-25.0, 0.4, -2.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);

	this->addObject("Resources/Models/grass2.lu", btVector3(1.0, 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(2.0, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(3.0, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(4, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(5, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(6, 0.4, -0.5), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(7, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(8, 0.4, -0.5), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(9, 0.4, -0.1), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(10, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(11, 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(12, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(13., 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(14., 0.4, -0.1), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(15, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(16, 0.4, 0.6), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(17, 0.4, -0.7), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(18, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(19, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(20, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(21, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(22, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(23, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(24, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(25, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(26, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);

	this->addObject("Resources/Models/grass2.lu", btVector3( 0.4, 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-1.7, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-2.4, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-3.6, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-4.2, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-5.8, 0.4, -0.5), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-6.5, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-7.3, 0.4, -0.5), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-8.6, 0.4, -0.1), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-9.8, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-10.4, 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-11.7, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-12.2, 0.4, -0.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-13.3, 0.4, -0.1), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-14.8, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-15.4, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-16.5, 0.4, -0.7), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-17.8, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-18.2, 0.4, -0.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-19.6, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-20.3, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-21.6, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-22.9, 0.4, -0.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-23.3, 0.4, -0.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(-24.4, 0.4, -0.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(-25.2, 0.4, -0.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);

	this->addObject("Resources/Models/grass2.lu", btVector3(1.5, 0.4, -1.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(2.5, 0.4, -1.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(3.5, 0.4, -1.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(4.5, 0.4, -2.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(5.5, 0.4, -1.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(6.5, 0.4, -2.5), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(7.5, 0.4, -1.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(8.5, 0.4, -2.5), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(9.5, 0.4, -1.1), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(10.5, 0.4, -2.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(11.5, 0.4, -1.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(12.5, 0.4, -2.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(13.5, 0.4, -1.4), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(14.5, 0.4, -2.1), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(15.5, 0.4, -2.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(16.5, 0.4, 1.6), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(17.5, 0.4, -1.7), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(18.5, 0.4, -1.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(19.5, 0.4, -1.2), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(20.5, 0.4, -2.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(21.5, 0.4, -2.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(22.5, 0.4, -1.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(23.5, 0.4, -2.0), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(24.5, 0.4, -1.3), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass2.lu", btVector3(25.5, 0.4, -1.8), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);
	this->addObject("Resources/Models/grass1.lu", btVector3(26.5, 0.4, -2.9), 0, 0, btVector3(1.0, 1.0, 1.0), BACKGROUND, GRASS, 1, false);



	//Background trees
	/*GameObject* tree1 = new GameObject;
	System::theModelLoader->loadGO(tree1, "Resources/Models/tree1.lu");
	System::handler->addObject(tree1);
	tree1->setPosition(10, 0, 7.6);
	tree1->setScale(1.6, 1.6, 1.6);

	tree1->setRotationRollPitchYaw(0, -1, 0);*/

	GameObject* tree2 = new GameObject;
	System::theModelLoader->loadGO(tree2, "Resources/Models/tree2.lu");
	System::handler->addObject(tree2);
	tree2->setPosition(-1, -0.5, 25);
	tree2->setScale(0.6, 0.6, 0.6);

	GameObject* tree3 = new GameObject;
	System::theModelLoader->loadGO(tree3, "Resources/Models/tree3.lu");
	System::handler->addObject(tree3);
	tree3->setPosition(-40, 0, 25);
	tree3->setScale(0.8, 0.8, 0.8);
	tree3->setRotationRollPitchYaw(0, -2.5, 0);

	//Background stones
	GameObject* cliff1 = new GameObject;
	System::theModelLoader->loadGO(cliff1, "Resources/Models/cliff1.lu");
	System::handler->addObject(cliff1);
	cliff1->setPosition(35, 0, 7.6);
	cliff1->setScale(10, 10, 10);

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
	
	/*GameObject* smallStone1 = new GameObject;
	System::theModelLoader->loadGO(smallStone1, "Resources/Models/small_stone1.lu");
	System::handler->addObject(smallStone1);
	smallStone1->setPosition(4, 0.5, 0);
	smallStone1->setScale(6, 6, 6);
	smallStone1->getRigidbody() = System::getphysices()->addBox(btVector3(0, 0, 0), btVector3(smallStone1->getScale().x, smallStone1->getScale().y, smallStone1->getScale().z), 10.0f, smallStone1->);
*/

	//Miscellaneous objects
	/*GameObject* grass1 = new GameObject;
	System::theModelLoader->loadGO(grass1, "Resources/Models/grass1.lu");
	System::handler->addObject(grass1);
	grass1->setPosition(-5, 0.6, -1);
	GameObject* grass2 = new GameObject;
	System::theModelLoader->loadGO(grass2, "Resources/Models/grass2.lu");
	System::handler->addObject(grass2);
	grass2->setPosition(-5, 0.6, 1);
	GameObject* grass3 = new GameObject;
	System::theModelLoader->loadGO(grass3, "Resources/Models/grass2.lu");
	System::handler->addObject(grass3);
	grass3->setPosition(5, 0.6, 1);
	GameObject* grass4 = new GameObject;
	System::theModelLoader->loadGO(grass4, "Resources/Models/grass2.lu");
	System::handler->addObject(grass4);
	grass4->setPosition(8, 0.6, -1.2);
	GameObject* grass5 = new GameObject;
	System::theModelLoader->loadGO(grass5, "Resources/Models/grass2.lu");
	System::handler->addObject(grass5);
	grass5->setPosition(7.5, 0.6, -5);
	GameObject* grass6 = new GameObject;
	System::theModelLoader->loadGO(grass6, "Resources/Models/grass2.lu");
	System::handler->addObject(grass6);
	grass6->setPosition(5.3, 0.6, -6);*/

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


	renderImgui();
	System::handler->draw(System::fusk->getDeltaTime(), this->paused, currentAnimSpeed, currentAnimName);

	System::shaderManager->getParticleShader()->setCBuffers();
	System::shaderManager->getParticleShader()->setShaders();
	System::shaderManager->getParticleShader()->setViewProj(System::theCamera->GetViewMatrix(), System::theGraphicDevice->getProj(), DirectX::XMFLOAT4(System::theCamera->GetPosition().x, System::theCamera->GetPosition().y, System::theCamera->GetPosition().z, 1.0f));
    System::getParticleManager()->render();

	System::fusk->resetShaders();

	if (this->resultsShown)
	{
		this->resultGui->render();
	}
	else
	{
		this->inGameGui->render();

		if (this->paused)
		{
			this->pauseGui->render();
		}
	}

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
		for (int j = 0; j < nrOfPlayers; j++) {
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
					System::getParticleManager()->addSimpleEffect(player[i]->getPosition(), "splat",1.0f);

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
						if(player[j]->canChange())
							player[j]->changeCharacter();
						else
						{
							this->resultsShown = true;
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
					if (Intersects(minTempObj, maxTempObj, player[j]->hitbox.hitbox->getCollisionBox(), player[j]->hitbox.hitbox->getPosition()) && !player[j]->getHitStun()) {
						objects[i]->addImpulse(player[j]->dir * 65 * player[j]->getWeight());
					}
					//if (Intersects(minTempObj, maxTempObj, player[j]->getAABB(),player[j]->getPosition())) {
					//	if (/*(object[i]->getMovingSpeed().x > 0.01 || object[i]->getMovingSpeed().y > 0.01) && !player[i]->getHitStun()*/true) {
					//		player[j]->setHitStun(true);
					//		//player[j]->playerObj->getRigidbody()->applyCentralImpulse(btVector3(player[j]->dir * 150 * ((player[j]->getWeight() + 2) / 3), 150 * ((player[j]->getWeight() + 2) / 3), 0));// , btVector3(1, 0, 0));

					//		//int tempHP = player[i]->getHealth();
					//		player[j]->takeDamage(10);
					//		System::getParticleManager()->addSimpleEffect(player[i]->getPosition());

					//		int randomNumber = (rand() % 4) + 0;
					//		System::getSoundManager()->playEffect(to_string(randomNumber));

					//		int randomNumber2 = (rand() % 3) - 1;
					//		int randomNumber3 = (rand() % 3) - 1;
					//		int randomNumber4 = (rand() % 3) - 1;

					//		if (randomNumber2 == 0 && randomNumber3 == 0 && randomNumber4 == 0) {
					//			int randomNumber2 = (rand() % 3) - 1;
					//			int randomNumber3 = (rand() % 3) - 1;
					//			int randomNumber4 = (rand() % 3) - 1;
					//		}

					//		System::theCamera->cameraShake(0.1, DirectX::XMFLOAT3(player[j]->dir, randomNumber3, randomNumber4));

					//	}
					//}
				}
			}
		}
		//GRASS ROTATION
		for (int i = 0; i < nrOfObjects; i++) {
			for (int j = 0; j < nrOfPlayers; j++) {
				if (objects[i]->GetType() == GRASS) {
					if ((((objects[i]->getPosition().x - player[j]->getPosition().x) < 2) && ((objects[i]->getPosition().x - player[j]->getPosition().x) > -2)) && player[j]->getPosition().y < 2) {
						objects[i]->addGrassRotation(0.005, player[j]->dir);
					}
				}
			}
			for (int j = 0; j < nrOfObjects; j++) {
				if (objects[i]->GetType() == GRASS && objects[j]->GetState() == TRUE_DYNAMIC) {
					if ((((objects[i]->getPosition().x - objects[j]->getRigidBodyPosition().getX()) < 1) && ((objects[i]->getPosition().x - objects[j]->getRigidBodyPosition().getX()) > -1)) && objects[j]->getRigidBodyPosition().getY() < 2) {
						objects[i]->addGrassRotation(0.01, objects[j]->getMovingDirection());
					}
				}
			}
		}

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
		this->objects[i]->respawn();
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
