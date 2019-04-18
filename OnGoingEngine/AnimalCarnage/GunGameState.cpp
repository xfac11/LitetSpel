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
	for (int i = 0; i < 4; i++)
	{
		theRumble[i].rumbleClock = 0.f;
		theRumble[i].rumbleTime = 0.f;
		theRumble[i].rumble = { 0.f,0.f };
		player[i].isJumping = false;
		player[i].airTimer = 0.f;
	}
}

GunGameState::~GunGameState()
{
}

bool GunGameState::initailize()
{
	return true;
}

bool GunGameState::render()
{

	return true;
}

bool GunGameState::update(float deltaTime)
{
	DirectX::GamePad::State state;


	for (int i = 0; i < 4; i++)
	{
		state = System::theGamePad->GetState(i);
		if (state.IsConnected())
		{
			System::theTracker->Update(state);

			//movement
			float stickAbsL = abs(state.thumbSticks.leftX);
			if (stickAbsL > 0.f)
			{
				float dir = 5 * state.thumbSticks.leftX;// / stickAbsL;

				this->player[i].direction = DirectX::XMFLOAT3(dir * deltaTime, 0, 0);

			}
			else if (state.dpad.right || state.dpad.left)
			{
				this->player[i].direction = DirectX::XMFLOAT3((state.dpad.right - state.dpad.left) * deltaTime, 0, 0);

			}
			else if (this->player[i].isJumping == false)
				this->player[i].direction = { 0,0,0 };
			else
				this->player[i].direction.y = 0; //= { 0,0,0 };


			//jump
			if (state.buttons.x && player[i].grounded == true ||
				state.buttons.y && player[i].grounded == true)//== DirectX::GamePad::ButtonStateTracker::PRESSED 
			{
				//
				this->player[i].isJumping = true;
				this->player[i].grounded = false;
			}

			//falling/jump function
			if (this->player[i].grounded == false)
			{
				this->player[i].airTimer += deltaTime;
				//continue here
			}
			this->player[i].direction.y += (-9.82f * this->player[i].airTimer + (5.f*this->player[i].isJumping)) * deltaTime;

			if (this->player[i].grounded == true)
			{
				this->player[i].airTimer = 0.f;
				this->player[i].isJumping = false;

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

			this->updateRumble(deltaTime,i);
		}
	}
	return true;
}

void GunGameState::shutDown()
{
}

DirectX::XMFLOAT3 GunGameState::getDirection(int id)
{
	return player[id].direction;
}

bool GunGameState::controllerIsConnected(int controllerPort)
{
	return System::theGamePad->GetState(controllerPort).IsConnected();
}
