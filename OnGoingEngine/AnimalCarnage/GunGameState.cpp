#include "GunGameState.h"
#include"System.h"

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

bool GunGameState::checkReset(DirectX::GamePad::State state)
{

	bool result = false;
	//if (state.IsConnected())
	//{
		//tracker.Update(state);
	if (((state.IsLeftTriggerPressed() && state.IsRightTriggerPressed()) ||
		(state.buttons.leftShoulder && state.buttons.rightShoulder)) &&
		state.buttons.a && (state.buttons.back || state.buttons.menu))
	{
		result = true;
	}
	//}
	return result;
}

GunGameState::GunGameState()
{
	
}

GunGameState::~GunGameState()
{
	shutDown();
}

bool GunGameState::initailize()
{

	nrOfPlayers = 4;
	player = new Player * [nrOfPlayers];

	for (int i = 0; i < nrOfPlayers; i++)
	{
		player[i] = new Player();
		player[i]->initialize();
	}
	System::handler.initialize();

	float pos[4] = {
	0,0,0,10.0
	};
	float dir[4] = {
		-1.0f,-1.0f,0.0f,1.0f
	};
	float color[4] = {
		0.9 , 1.0f, 0.2f , 1.0f
	};
	System::handler.addLight(pos, dir, color);
	float color2[4] = {
		0.0 , 1.0f, 1.0f , 1.0f
	};
	System::handler.addLight(pos, dir, color2);


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
	return true;
}

bool GunGameState::render()
{
	renderImgui();
	System::handler.draw();

	//for (int i = 0; i < bodies.size(); i++)
	//{
	//	
	//		renderSphere(bodies[i]);
	//	
	//}
	//world->stepSimulation(1 / 60.0);

	//this->gui->render();
	return true;
}

void GunGameState::renderImgui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	std::string textUse;
	ImGui::Begin("Gungame");


	//ImGui::EndChild();
	ImGui::CaptureKeyboardFromApp(true);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

bool GunGameState::update(float deltaTime)
{

	for (int i = 0; i < nrOfPlayers; i++)
	{
		player[i]->update(deltaTime, i);
		player[i]->updateRumble(deltaTime, i);
	}

	if (Intersects(System::handler.getObject(2).getCollisionBox(), System::handler.getObject(2).getPosition(), System::handler.getObject(3).getCollisionBox(), System::handler.getObject(3).getPosition()))
	{

	}
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

}



bool GunGameState::controllerIsConnected(int controllerPort)
{
	return System::theGamePad->GetState(controllerPort).IsConnected();
}

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

bool GunGameState::collision(DirectX::XMFLOAT2 posOne, float radiusOne, DirectX::XMFLOAT2 posTwo, float radiusTwo)
{
	bool result = false;
	float deltaX = posTwo.x - posOne.x;
	float deltaY = posTwo.y - posOne.y;
	float rad = radiusOne + radiusTwo;
	if ((deltaX * deltaX) + (deltaY * deltaY) < rad * rad)
	{
		result = true;
	}
	return result;
}

bool GunGameState::collision(DirectX::XMFLOAT2 posBox, DirectX::XMFLOAT2 scaleBox, DirectX::XMFLOAT2 posCircle, float radiusCircle)
{
	float size = 0.5f;
	float widthBox = scaleBox.x * size * 2;
	float heightBox = scaleBox.y * size * 2;


	float Dx = posCircle.x - std::fmaxf(posBox.x, std::fminf(posCircle.x, posBox.x + widthBox));
	float Dy = posCircle.y - std::fmaxf(posBox.y, std::fminf(posCircle.y, posBox.y + heightBox));

	return (Dx * Dx + Dy * Dy) < (radiusCircle * radiusCircle);
}
