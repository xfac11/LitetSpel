#include "GunGameState.h"
#include"System.h"

bool GunGameState::checkReset(DirectX::GamePad::State state)
{

	bool result = false;
	//if (state.IsConnected())
	//{
		//tracker.Update(state);
		if (((state.IsLeftTriggerPressed() && state.IsRightTriggerPressed())|| 
			(state.buttons.leftShoulder && state.buttons.rightShoulder) )&&
			state.buttons.a &&(state.buttons.back || state.buttons.menu))
		{
			result = true;
		}
	//}
	return result;
}

GunGameState::GunGameState()
{
	
	for (int i = 0; i < 2; i++)
	{
		items[i].isFlying = false;
		items[i].airTimer = 0.f;
		items[i].grounded = true;
		items[i].lastDir = 1;
		items[i].weight = 2.f;
	}
	
}

GunGameState::~GunGameState()
{
	shutDown();
}

bool GunGameState::initailize()
{

	nrOfPlayers = 4;
	player = new Player*[nrOfPlayers];

	for (int i = 0; i < nrOfPlayers; i++)
	{
		player[i] = new Player();
		player[i]->initialize();
	}
	return true;
}

bool GunGameState::render()
{
	System::handler.draw();
	//this->gui->render();
	return true;
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
	float widthBox = scaleBox.x*size * 2;
	float heightBox = scaleBox.y*size * 2;


	float Dx = posCircle.x - std::fmaxf(posBox.x, std::fminf(posCircle.x, posBox.x + widthBox));
	float Dy = posCircle.y - std::fmaxf(posBox.y, std::fminf(posCircle.y, posBox.y + heightBox));

	return (Dx * Dx + Dy * Dy) < (radiusCircle * radiusCircle);
}
