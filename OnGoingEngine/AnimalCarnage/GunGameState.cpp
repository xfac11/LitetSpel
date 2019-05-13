#include "GunGameState.h"
#include "System.h"
#include "GunGameGui.h"
#include "PauseGui.h"

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
	this->inGameGui = nullptr;
	this->pauseGui = nullptr;
	this->cameraFocus = 0;

	this->objectId = 1;
}

GunGameState::~GunGameState()
{
	shutDown();
	delete object[0];
	delete object[1];
	delete object[2];
	delete object[3];
	delete object[4];
	delete object[5];
}

//void GunGameState::callback(int other_arg, void * this_pointer)
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

bool GunGameState::callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{

	if (obj1->getCollisionObject()->getUserPointer() == (Player*)obj1->getCollisionObject()->getUserPointer())
	{
		if (((Player*)obj1->getCollisionObject()->getUserPointer()) != nullptr) {
			bool ishit = ((Player*)obj1->getCollisionObject()->getUserPointer())->getHit();
		}
		//object[3]->getId();
		int i;
		Objects* pointer = (Objects*)obj1->getCollisionObject()->getUserPointer();
		i=pointer->getId();
		/*if (i ) {
			i = ((Objects*)obj2->getCollisionObject()->getUserPointer())->getId();
		}*/
		
		if (i == 3) {
			
			OutputDebugStringA("1\n");
			if (((Player*)obj2->getCollisionObject()->getUserPointer()) != nullptr) {
				((Player*)obj2->getCollisionObject()->getUserPointer())->setGrounded(true);
			}
		}
		/*if (i == 2) {
			OutputDebugStringA("1\n");
			if (((Player*)obj2->getCollisionObject()->getUserPointer()) != nullptr) {
				((Player*)obj2->getCollisionObject()->getUserPointer())->setGrounded(true);
			}
		}*/
		/*else {
			((Player*)obj1->getCollisionObject()->getUserPointer())->setGrounded(false);
		}*/
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

bool GunGameState::initailize()
{
	gContactAddedCallback = callbackFunc;

	this->object[0] = new Objects("Resources/Models/cube2.lu", btVector3(0, 8, 0),3,3, btVector3(5.f, 1.f, 1.f));
	this->object[1] = new Objects("Resources/Models/cube2.lu", btVector3(9, 4, 0), 3,3, btVector3(5.f, 1.f, 1.f), DYNAMIC);
	this->object[2] = new Objects("Resources/Models/cube2.lu", btVector3(5,4, 0), 3,3, btVector3(5.f, 1.f, 1.f),DYNAMIC);
	this->object[3] = new Objects("Resources/Models/cube2.lu", btVector3(16, -2, 0), 3,3, btVector3(100.f, 4.f, 10.f), STATIC);
	this->object[4] = new Objects("Resources/Models/cube2.lu", btVector3(35, 17, 0), 2,1, btVector3(10.f, 40.f, 10.f), STATIC);
	this->object[5] = new Objects("Resources/Models/cube2.lu", btVector3(-35, 17, 0), 2,1, btVector3(10.f, 40.f, 10.f), STATIC);
	//this->object[3] = new Objects("Resources/Models/cube2.lu", "stones_and_rocks_diffuse_base.tga", btVector3(3, 2, 0));

	/*//->setLinearFactor(btVector3(0,0,0));
	ground = new GameObject(System::shaderManager->getForwardShader());
	System::theModelLoader->loadGO(ground, "Resources/Models/cube2.lu");

	ground->setScale(100,2,25); 
	ground->getRigidbody() = System::getphysices()->addBox(btVector3(0, -3, 5), btVector3(ground->getScale().x, ground->getScale().y, ground->getScale().z),0);
	ground->getRigidbody()->setLinearFactor(btVector3(0,0,0));
	//ground->setScale(ground->getRigidbody()->get);
	this->ground->getRigidbody()->setFriction(3);
	ground->getRigidbody()->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);	
	System::handler->addObject(ground);
	this->ground->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);

	wall1 = new GameObject(System::shaderManager->getForwardShader());
	wall1->setScale(2, 20, 20);
	wall1->getRigidbody() = System::getphysices()->addBox(btVector3(-24, 6, 0), btVector3(wall1->getScale().x, wall1->getScale().y, ground->getScale().z), 0);
	wall1->getRigidbody()->setLinearFactor(btVector3(0, 0, 0));
	this->wall1->getRigidbody()->setFriction(0.5);
	wall1->getRigidbody()->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
	System::theModelLoader->loadGO(wall1, "Resources/Models/cube2.lu");
	System::handler->addObject(wall1);
	this->wall1->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);

	wall2 = new GameObject(System::shaderManager->getForwardShader());
	wall2->setScale(2, 20, 20);
	wall2->getRigidbody() = System::getphysices()->addBox(btVector3(24, 6, 0), btVector3(2, 20, 20), 0);
	wall2->getRigidbody()->setLinearFactor(btVector3(0, 0, 0));
	this->wall2->getRigidbody()->setFriction(0.5);
	wall2->getRigidbody()->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
	System::theModelLoader->loadGO(wall2, "Resources/Models/cube2.lu");
	System::handler->addObject(wall2);
	this->wall2->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
	*/
	
	GameObject* tree1 = new GameObject;
	System::theModelLoader->loadGO(tree1, "Resources/Models/tree2.lu");
	System::handler->addObject(tree1);
	tree1->setPosition(0, -0.5, 10);
	tree1->setScale(0.6, 0.6, 0.6);
	GameObject* tree2 = new GameObject;
	System::theModelLoader->loadGO(tree2, "Resources/Models/small_stone1.lu");
	System::handler->addObject(tree2);
	tree2->setPosition(2, 0.5, 0);
	tree2->setScale(10, 5, 5);
	GameObject* tree = new GameObject;
	System::theModelLoader->loadGO(tree, "Resources/Models/grass2.lu");
	System::handler->addObject(tree);
	tree->setPosition(10, -0.5, 1);
	//tree->setScale(0.8, 0.8, 0.8);
	//tree->setRotationRollPitchYaw(0,1,0);


	nrOfPlayers = 4;
	player = new Player * [nrOfPlayers];

	for (int i = 0; i < nrOfPlayers; i++)
	{
		player[i] = new Player();
		player[i]->initialize();
		player[i]->setRigidbodyPosition(0.f, i *10.f, 0.f);
	}
	
	System::handler->initialize();
	System::handler->setSkyboxTexture("painted_skybox2");
	float pos[4] = {
	0.f ,0.3f ,0.f,10.f
	};
	float dir[4] = {
		-1.0f,-1.0f,0.0f,1.0f
	};
	float color[4] = {
		1.0f , 1.0f, 1.0f , 1.0f
	};
	System::handler->addLight(pos, dir, color);
	float color2[4] = {
		0.5f , 0.0f, 1.0f , 1.0f
	};
	System::handler->addLight(pos, dir, color2);
	color2[0] = 0.0f;
	color2[1] = 1.0f;
	color2[2] = 1.0f;
	pos[1] = 5.0f;
	color2[3] = 10.0f;
	System::handler->addLight(pos, dir, color2);
	//player[0]->playerObj->body()->getWorldTransform().setOrigin(btVector3(-2, 2, 0));
	//player[1]->playerObj->body()->getWorldTransform().setOrigin(btVector3(0, 2, 0));
	//player[2]->playerObj->body()->getWorldTransform().setOrigin(btVector3(2, 2, 0));
	//player[3]->playerObj->body()->getWorldTransform().setOrigin(btVector3(4, 2, 0));


	////default values
	//this->collisionConfig = new btDefaultCollisionConfiguration();
	//dispatcher = new btCollisionDispatcher(collisionConfig);
	//this->broadphase = new btDbvtBroadphase();
	//this->solver = new btSequentialImpulseConstraintSolver();

	//world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	//world->setGravity(btVector3(0, -10, 0));

	////add object set transform
	//btTransform t; //
	//t.setIdentity();
	//t.setOrigin(btVector3(0, 0, 0));
	////set shape for object
	//btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	////set motionshape aka set postion
	//btMotionState* motion = new btDefaultMotionState(t);
	////body definition check doc
	//btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);

	//btRigidBody* body = new btRigidBody(info);

	//world->addRigidBody(body);
	//bodies.push_back(body);

	////add sphere
	//
	////addSphere(1.0f, 0, 20, 0, 1.0);

	this->inGameGui = new GunGameGui(this);
	this->inGameGui->initialize();
	this->pauseGui = new PauseGui(this);
	this->pauseGui->initialize();

	return true;
}

bool GunGameState::render()
{
	renderImgui();
	System::handler->draw();

	System::fusk->resetShaders();
	this->inGameGui->render();

	if (paused)
	{
		this->pauseGui->render();
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

	OutputDebugStringA("0\n");
	if (paused)
	{
		this->pauseGui->update(deltaTime);
		return true;
	}

	for (int i = 0; i < 3; i++)
	{
		this->object[i]->update(deltaTime);
	}

	/*ground->setPosition(ground->getRigidbody()->getWorldTransform().getOrigin().getX(), ground->getRigidbody()->getWorldTransform().getOrigin().getY()+1.6f, ground->getRigidbody()->getWorldTransform().getOrigin().getZ());
	ground->getRigidbody()->setLinearFactor(btVector3(0, 0, 0));
	ground->getRigidbody()->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);

	wall1->setPosition(wall1->getRigidbody()->getWorldTransform().getOrigin().getX(), wall1->getRigidbody()->getWorldTransform().getOrigin().getY(), wall1->getRigidbody()->getWorldTransform().getOrigin().getZ());
	wall2->setPosition(wall2->getRigidbody()->getWorldTransform().getOrigin().getX(), wall2->getRigidbody()->getWorldTransform().getOrigin().getY(), wall2->getRigidbody()->getWorldTransform().getOrigin().getZ());
	*/
	for (int i = 0; i < nrOfPlayers; i++)
	{
		btVector3 min;
		btVector3 max;
		player[i]->playerObj->getRigidbody()->getAabb(min, max);
		DirectX::XMFLOAT3 minTemp(min.getX(), min.getY(), min.getZ());
		DirectX::XMFLOAT3 maxTemp(max.getX(), max.getY(), max.getZ());
		for (int j = 0; j < nrOfPlayers; j++) {
			if (i != j)
			{
				//if (Intersects(minTemp, maxTemp, player[j]->playerObj->getCollisionBox(), player[j]->playerObj->getPosition()))
				//	player[i]->setGrounded(true);
				//if(player[i]->playerObj->getRigidbody()->checkCollideWith(player[j]->playerObj->getRigidbody()))
				//	player[i]->setGrounded(true);

				//if(player[i]->playerObj->getRigidbody()->checkCollideWithOverride(player[j]->playerObj->getRigidbody()))
				//	player[i]->setGrounded(true);
				if (Intersects(minTemp, maxTemp, player[j]->hitbox.hitbox->getCollisionBox(), player[j]->hitbox.hitbox->getPosition()))
				{
				
					this->testColBox = true;
					player[i]->playerObj->getRigidbody()->applyCentralImpulse(btVector3(player[j]->dir * 50, 25, 0));// , btVector3(1, 0, 0));
					//player[i]->setGrounded(true);
				}
				else
				{
					this->testColBox = false;
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
	if (Intersects(System::handler->getObject(2).getCollisionBox(), System::handler->getObject(2).getPosition(), System::handler->getObject(3).getCollisionBox(), System::handler->getObject(3).getPosition()))
	{

	}

	this->inGameGui->update(deltaTime);
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
	for (int i = 0; i < nrOfPlayers; i++)
	{
		delete player[i];
	}
	delete[] player;

	delete this->inGameGui;
	delete this->pauseGui;
	this->inGameGui = nullptr;
	this->pauseGui = nullptr;
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

//btCollisionObjectWrapper GunGameState::getGroundCollisionObject()
//{
//	//return ground->;
//}

//bool GunGameState::collision(DirectX::XMFLOAT2 posOne, DirectX::XMFLOAT2 scaleOne,int playerID, DirectX::XMFLOAT2 posTwo, DirectX::XMFLOAT2 scaleTwo, int itemID)
//{
//	bool result = false;
//
//	DirectX::GamePad::State state;
//
//	state = System::theGamePad->GetState(playerID);
//	if (state.IsConnected())
//	{
//		System::theTracker->Update(state);
//		if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
//		{
//
//			float size = 0.5f;
//
//			DirectX::XMFLOAT2 topLeftOne = { posOne.x - scaleOne.x * size, posOne.y - size * scaleOne.y };
//			DirectX::XMFLOAT2 topLeftTwo = { posTwo.x - scaleTwo.x * size, posTwo.y - size * scaleTwo.y };
//
//			if (topLeftOne.x < topLeftTwo.x + (scaleTwo.x*size * 2) &&
//				topLeftOne.x + (scaleOne.x*size * 2) > topLeftTwo.x &&
//				topLeftOne.y < topLeftTwo.y + (scaleTwo.y *size * 2) &&
//				topLeftOne.y + (scaleOne.y*size * 2) > topLeftTwo.y)
//			{
//				result = true;
//				items[itemID].isFlying = true;
//				items[itemID].grounded = false;
//			}
//		}
//	}
//	
//	return result;
//}

//bool GunGameState::collision(DirectX::XMFLOAT2 posOne, float radiusOne, DirectX::XMFLOAT2 posTwo, float radiusTwo)
//{
//	bool result = false;
//	float deltaX = posTwo.x - posOne.x;
//	float deltaY = posTwo.y - posOne.y;
//	float rad = radiusOne + radiusTwo;
//	if ((deltaX * deltaX) + (deltaY * deltaY) < rad * rad)
//	{
//		result = true;
//	}
//	return result;
//}
//
//bool GunGameState::collision(DirectX::XMFLOAT2 posBox, DirectX::XMFLOAT2 scaleBox, DirectX::XMFLOAT2 posCircle, float radiusCircle)
//{
//	float size = 0.5f;
//	float widthBox = scaleBox.x * size * 2;
//	float heightBox = scaleBox.y * size * 2;
//
//
//	float Dx = posCircle.x - std::fmaxf(posBox.x, std::fminf(posCircle.x, posBox.x + widthBox));
//	float Dy = posCircle.y - std::fmaxf(posBox.y, std::fminf(posCircle.y, posBox.y + heightBox));
//
//	return (Dx * Dx + Dy * Dy) < (radiusCircle * radiusCircle);
//}
