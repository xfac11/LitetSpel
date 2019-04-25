#include "GunGameState.h"
#include"System.h"

GunGameState::GunGameState() 
{
	for (int i = 0; i < 4; i++) //temp players
	{
		tplayer[i].isJumping = false;
		tplayer[i].canJump = true;
		tplayer[i].airTimer = 0.f;
		tplayer[i].airSpeed = 0.f;
	}
	for (int i = 0; i < 2; i++)
	{
		items[i].isFlying = false;
		items[i].airTimer = 0.f;
		items[i].grounded = true;
		items[i].lastDir = 1;
		items[i].weight = 2.f;
	}
	this->player = nullptr;
	this->nrOfPlayers = 0;
}

GunGameState::~GunGameState()
{
	shutDown();
}

bool GunGameState::initailize()
{
	player = new Player * [nrOfPlayers];
	return true;
}

bool GunGameState::render()
{
	//this->gui->render();
	return true;
}

bool GunGameState::update(float deltaTime)
{
	for (int i = 0; i < 4; i++) //nrOfPlayers
	{
		DirectX::GamePad::State state = System::theGamePad->GetState(i);
		if (state.IsConnected())
		{
			System::theTracker->Update(state);
			//Actions 

			if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED) //and
			{
				System::theRumble[i].rumble.x = 0.6f;
				System::theRumble[i].rumble.y = 0.6f;
				System::theRumble[i].rumbleTime = 0.3f;
			}
			if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::HELD) //and
			{
				System::theRumble[i].rumble.x = 0.2f;
				System::theRumble[i].rumble.y = 0.2f;
				System::theRumble[i].rumbleTime = 0.2f;
			}
			//if (System::theTracker->dpadUp == DirectX::GamePad::ButtonStateTracker::RELEASED) //and
			//{
			//	System::theRumble[i].rumble.x = 0.2f;
			//	System::theRumble[i].rumble.y = 0.2f;
			//	System::theRumble[i].rumbleTime = 0.2f;
			//}


			//GROUND MOVEMENT
			float stickAbsL = abs(state.thumbSticks.leftX);
			if (stickAbsL > 0.f && tplayer[i].grounded)
			{
				float dir = 5 * state.thumbSticks.leftX;// / stickAbsL;

				this->tplayer[i].direction = DirectX::XMFLOAT3(dir * deltaTime, 0, 0);
				tplayer[i].airSpeed = dir;
			}
			else if ((state.dpad.right || state.dpad.left) && tplayer[i].grounded)
			{
				this->tplayer[i].direction = DirectX::XMFLOAT3(5 * (state.dpad.right - state.dpad.left) * deltaTime, 0, 0);
				tplayer[i].airSpeed = 5.f * (state.dpad.right - state.dpad.left);
			}
			else if (this->tplayer[i].isJumping == false)
				this->tplayer[i].direction = { 0,0,0 };
			else
				this->tplayer[i].direction.y = 0; //= { 0,0,0 };

			//IN AIR MOVEMENT
			float airspeedAbs = abs(tplayer[i].airSpeed);
			if (stickAbsL > 0.f && !tplayer[i].grounded) 
			{
				tplayer[i].airSpeed += 0.5f * state.thumbSticks.leftX;
				float dir = tplayer[i].airSpeed;// / stickAbsL;

				this->tplayer[i].direction = DirectX::XMFLOAT3(dir * deltaTime, 0, 0);

				if (airspeedAbs > 5)
					tplayer[i].airSpeed = airspeedAbs / tplayer[i].airSpeed*5;
			}
			else if (state.dpad.right && !tplayer[i].grounded || state.dpad.left && !tplayer[i].grounded)
			{
					tplayer[i].airSpeed += 0.5f * (state.dpad.right - state.dpad.left);

				if (!tplayer[i].grounded) {
					this->tplayer[i].direction = DirectX::XMFLOAT3(tplayer[i].airSpeed * deltaTime, 0, 0);
				}
	
				if (airspeedAbs > 5)
					tplayer[i].airSpeed = airspeedAbs / tplayer[i].airSpeed*5;
			}
			
			

			//JUMP INPUT
			if ((state.buttons.x || state.buttons.y) && tplayer[i].canJump)
			{
				this->tplayer[i].isJumping = true;
				this->tplayer[i].grounded = false;
			}

			//canJump
			if (state.buttons.x || state.buttons.y) {
				tplayer[i].canJump = false;
			}
			if (tplayer[i].grounded /*&& System::theTracker->x == DirectX::GamePad::ButtonStateTracker::RELEASED || System::theTracker->y == DirectX::GamePad::ButtonStateTracker::RELEASED*/) {
				tplayer[i].canJump = true;
			}



			//falling/jump function
			if (this->tplayer[i].grounded == false)
			{
				this->tplayer[i].airTimer += deltaTime;
				this->tplayer[i].direction.y += (-9.82f * this->tplayer[i].airTimer + (5.f*this->tplayer[i].isJumping)) * deltaTime;
				//continue here
			}
			else if (this->tplayer[i].grounded == true)
			{
				this->tplayer[i].airTimer = 0.f;
				this->tplayer[i].isJumping = false;
				this->tplayer[i].direction.y = 0.f;
				this->tplayer[i].airSpeed = 0.0f;
			}


			if (state.buttons.leftShoulder ||
				state.buttons.rightShoulder)
			{
				System::theRumble[i].rumble.x = 0.4f;
				System::theRumble[i].rumble.y = 0.4f;
				System::theRumble[i].rumbleTime = 0.4f;
			}

			//pause
			if (state.buttons.menu)
			{
				//pause
				System::theRumble[i].rumble.x = 0.3f;
				System::theRumble[i].rumbleTime = 0.1f;
			}

			//exit(debug build)
			if (state.buttons.back)
			{
				//exit game
				System::theRumble[i].rumble.y = 0.3f;
				System::theRumble[i].rumbleTime = 0.1f;
			}

			//ITEMS not working properly  (does not leave ground)
			//for (int i = 0; i < 2; i++)
			//{
			//	if (state.dpad.right - state.dpad.left != 0)
			//		items[i].lastDir = state.dpad.right - state.dpad.left;
			//	else if (abs(state.thumbSticks.leftX) == 1)
			//		items[i].lastDir = state.thumbSticks.leftX;

			//	if (items[i].grounded==false)
			//	{
			//		this->items[i].airTimer += deltaTime;

			//		this->tplayer[i].direction.x += this->items[i].lastDir*deltaTime;
			//		this->tplayer[i].direction.y += (-9.82f *items[i].airTimer + (2.f*this->items[i].isFlying))*deltaTime;
			//	}
			//	else if (items[i].grounded == true)
			//	{
			//		this->items[i].airTimer = 0.f;
			//		this->items[i].isFlying = false;
			//		this->items[i].direction.y = 0;
			//		this->items[i].direction.x = 0;
			//	}
			//}
		}
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

bool GunGameState::isGrounded(int id)
{
	return tplayer[id].grounded;
}

void GunGameState::setGrounded(int id, bool condition)
{
	tplayer[id].grounded = condition;
}

void GunGameState::setObjGrounded(int id, bool condition)
{
	items[id].grounded = condition;
}

DirectX::XMFLOAT3 GunGameState::getDirection(int id)
{
	
	return tplayer[id].direction;
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
