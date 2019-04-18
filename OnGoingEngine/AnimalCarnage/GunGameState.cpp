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
		tplayer[i].isJumping = false;
		tplayer[i].airTimer = 0.f;
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

				this->tplayer[i].direction = DirectX::XMFLOAT3(dir * deltaTime, 0, 0);

			}
			else if (state.dpad.right || state.dpad.left)
			{
				this->tplayer[i].direction = DirectX::XMFLOAT3((state.dpad.right - state.dpad.left) * deltaTime, 0, 0);

			}
			else if (this->tplayer[i].isJumping == false)
				this->tplayer[i].direction = { 0,0,0 };
			else
				this->tplayer[i].direction.y = 0; //= { 0,0,0 };


			//jump
			if (state.buttons.x && tplayer[i].grounded == true ||
				state.buttons.y && tplayer[i].grounded == true)//== DirectX::GamePad::ButtonStateTracker::PRESSED 
			{
				//
				this->tplayer[i].isJumping = true;
				this->tplayer[i].grounded = false;
			}

			//falling/jump function
			if (this->tplayer[i].grounded == false)
			{
				this->tplayer[i].airTimer += deltaTime;
				//continue here
			}
			this->tplayer[i].direction.y += (-9.82f * this->tplayer[i].airTimer + (5.f*this->tplayer[i].isJumping)) * deltaTime;

	//for (int i = 0; i < nrOfPlayers; i++)
	//{
	//	player[i]->update();
	//	//swap characters
	//	//player[i]->SelectCharacter();
	//	for (int j = 0; j < nrOfPlayers; j++)
	//	{
	//		//collision detection
	//		if (Intersects(player[i]->GetBody(), player[j]->GetBody()))
	//		{
	//			//logic
	//		}
	//	}

	//}

			if (this->tplayer[i].grounded == true)
			{
				this->tplayer[i].airTimer = 0.f;
				this->tplayer[i].isJumping = false;

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
	for (int i = 0; i < nrOfPlayers; i++)
	{
		delete player[i];
	}
	delete[] player;
}

DirectX::XMFLOAT3 GunGameState::getDirection(int id)
{
	return tplayer[id].direction;
}

bool GunGameState::controllerIsConnected(int controllerPort)
{
	return System::theGamePad->GetState(controllerPort).IsConnected();
}
