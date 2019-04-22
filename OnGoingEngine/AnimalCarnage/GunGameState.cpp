#include "GunGameState.h"
#include"System.h"

void GunGameState::updateRumble(float deltaTime, int id)
{
	if (theRumble[id].rumbleTime > theRumble[id].rumbleClock)
	{
		theRumble[id].rumbleClock += deltaTime;
		System::theGamePad->SetVibration(0, theRumble[id].rumble.x, theRumble[id].rumble.y);
	}
	else
	{
		theRumble[id].rumbleClock = 0.f;
		theRumble[id].rumbleTime = 0.f;
		System::theGamePad->SetVibration(0, 0, 0);
	}
}

GunGameState::GunGameState() 
{
	for (int i = 0; i < 4; i++) //temp
	{
		theRumble[i].rumbleClock = 0.f;
		theRumble[i].rumbleTime = 0.f;
		theRumble[i].rumble = { 0.f,0.f };
		tplayer[i].isJumping = false;
		tplayer[i].canJump = true;
		tplayer[i].airTimer = 0.f;
		tplayer[i].airSpeed = 0.f;
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
	DirectX::GamePad::State state;

	for (int i = 0; i < 4; i++) //nrOfPlayers
	{
		state = System::theGamePad->GetState(i);
		if (state.IsConnected())
		{
			System::theTracker->Update(state);
					

			//movement
			float stickAbsL = abs(state.thumbSticks.leftX);
			if (stickAbsL > 0.f && tplayer[i].grounded)
			{
				float dir = 5 * state.thumbSticks.leftX;// / stickAbsL;

				this->tplayer[i].direction = DirectX::XMFLOAT3(dir * deltaTime, 0, 0);
				tplayer[i].airSpeed = dir;
			}
			else if (state.dpad.right || state.dpad.left && tplayer[i].grounded)
			{
				this->tplayer[i].direction = DirectX::XMFLOAT3(5 * (state.dpad.right - state.dpad.left) * deltaTime, 0, 0);
				tplayer[i].airSpeed = 5 * (state.dpad.right - state.dpad.left);
			}
			else if (this->tplayer[i].isJumping == false)
				this->tplayer[i].direction = { 0,0,0 };
			else
				this->tplayer[i].direction.y = 0; //= { 0,0,0 };

			//IN AIR MOVEMENT
			if (stickAbsL > 0.f && tplayer[i].grounded==false)
			{
				tplayer[i].airSpeed += 0.5f * state.thumbSticks.leftX;
				float dir = tplayer[i].airSpeed;// / stickAbsL;

				this->tplayer[i].direction = DirectX::XMFLOAT3(dir * deltaTime, 0, 0);

				if (tplayer[i].airSpeed > 5) {
					tplayer[i].airSpeed = 5;
				}
				else if (tplayer[i].airSpeed < -5) {
					tplayer[i].airSpeed = -5;
				}
			}
			else if (state.dpad.right || state.dpad.left && tplayer[i].grounded == false)
			{
				tplayer[i].airSpeed += 0.5f * (state.dpad.right - state.dpad.left);
				if (!tplayer[i].grounded) {
					this->tplayer[i].direction = DirectX::XMFLOAT3(tplayer[i].airSpeed * deltaTime, 0, 0);
				}
				if (tplayer[i].airSpeed > 5) {
					tplayer[i].airSpeed = 5;
				}
				else if (tplayer[i].airSpeed < -5) {
					tplayer[i].airSpeed = -5;
				}
			}


			//jump
			if ((state.buttons.x || state.buttons.y) && tplayer[i].canJump)//== DirectX::GamePad::ButtonStateTracker::PRESSED 
			{

				this->tplayer[i].isJumping = true;
				this->tplayer[i].grounded = false;
				//this->tplayer[i].grounded = false;
				
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
			

			//Actions 
			if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED) //and
			{
				theRumble[i].rumble.x = 1.f;
				theRumble[i].rumble.y = 1.f;
				theRumble[i].rumbleTime = 0.1f;
			}
			if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::PRESSED) //and
			{
				theRumble[i].rumble.x = 1.f;
				theRumble[i].rumble.y = 1.f;
				theRumble[i].rumbleTime = 0.1f;
			}

			if (state.buttons.leftShoulder ||
				state.buttons.rightShoulder)
			{
				theRumble[i].rumble.x = 0.1f;
				theRumble[i].rumble.y = 0.1f;
				theRumble[i].rumbleTime = 0.1f;
			}

			//pause
			if (state.buttons.menu)
			{
				//pause
				theRumble[i].rumble.x = 0.1f;
				theRumble[i].rumbleTime = 0.1f;
			}

			//exit(debug build)
			if (state.buttons.back)
			{
				//exit game
				theRumble[i].rumble.y = 0.1f;
				theRumble[i].rumbleTime = 0.1f;
			}

			this->updateRumble(deltaTime, i);
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

DirectX::XMFLOAT3 GunGameState::getDirection(int id)
{
	
	return tplayer[id].direction;
}

bool GunGameState::controllerIsConnected(int controllerPort)
{
	return System::theGamePad->GetState(controllerPort).IsConnected();
}

bool GunGameState::collision(DirectX::XMFLOAT2 posOne, DirectX::XMFLOAT2 scaleOne,int playerID, DirectX::XMFLOAT2 posTwo, DirectX::XMFLOAT2 scaleTwo, int itemID)
{
	bool result = false;

	DirectX::GamePad::State state;

	state = System::theGamePad->GetState(playerID);
	if (state.IsConnected())
	{
		System::theTracker->Update(state);
		if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
		{

			float size = 0.5f;

			DirectX::XMFLOAT2 topLeftOne = { posOne.x - scaleOne.x * size, posOne.y - size * scaleOne.y };
			DirectX::XMFLOAT2 topLeftTwo = { posTwo.x - scaleTwo.x * size, posTwo.y - size * scaleTwo.y };

			if (topLeftOne.x < topLeftTwo.x + (scaleTwo.x*size * 2) &&
				topLeftOne.x + (scaleOne.x*size * 2) > topLeftTwo.x &&
				topLeftOne.y < topLeftTwo.y + (scaleTwo.y *size * 2) &&
				topLeftOne.y + (scaleOne.y*size * 2) > topLeftTwo.y)
			{
				result = true;
				items[itemID].isFlying = true;
			}
		}
	}
	
	return result;
}

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
