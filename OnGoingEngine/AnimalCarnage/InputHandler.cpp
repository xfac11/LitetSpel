#include "InputHandler.h"

bool InputHandler::checkReset(int player)
{
	//if()
	bool result = false;
	if (theGamePad.GetState(player).IsLeftTriggerPressed() &&
		theGamePad.GetState(player).IsRightTriggerPressed() &&
		theGamePad.GetState(player).buttons.a&&
		theGamePad.GetState(player).buttons.back || theGamePad.GetState(player).buttons.menu)
	{
		result = true;
	}
	return result;
}

void InputHandler::inGameMode(float deltaTime,bool grounded)
{
	
	DirectX::GamePad::State state;


	//for()
	state = this->theGamePad.GetState(0);
	if (state.IsConnected())
	{
		tracker.Update(state);
		//move left/right
		float stickAbsL = abs(state.thumbSticks.leftX);
		if (stickAbsL > 0.f)
		{
			float dir = 5 * state.thumbSticks.leftX;// / stickAbsL;
			this->direction[0] = DirectX::XMFLOAT3(dir * deltaTime, 0, 0);

		}
		else if (state.dpad.right || state.dpad.left)
		{
			this->direction[0] = DirectX::XMFLOAT3((state.dpad.right - state.dpad.left) * deltaTime, 0, 0);

		}
		else
			this->direction[0] = { 0,0,0 };
		//jump
		if (tracker.x == DirectX::GamePad::ButtonStateTracker::PRESSED  && grounded == true ||
			tracker.y == DirectX::GamePad::ButtonStateTracker::PRESSED && grounded == true)
		{
			//
			this->isJumping=true;
		}
		else if(grounded == false)
			direction[0].y = -5.f * deltaTime;
	
		//jump function
		if (isJumping == true)
		{
			this->jumpTimer += deltaTime;
			if (this->jumpTimer < 1.f)
			{
				direction[0].y += (-9.82f * this->jumpTimer + 11.f) * deltaTime;
			}
			else
			{
				//this->rumble.x = 0.5f;
				//this->rumble.y = 0.5f;
				//this->rumbleTime = 0.1f;

				this->jumpTimer = 0.f;
				this->isJumping = false;
			}
		}

		//Action
		if (tracker.a == DirectX::GamePad::ButtonStateTracker::PRESSED)
		{
			rumble.x = 1.f;
			rumble.y = 1.f;
		}
		if (state.buttons.b)
		{
			rumble.x = 1.f;
			rumble.y = 1.f;
		}
		if (state.buttons.leftShoulder ||
			state.buttons.rightShoulder)
		{
			rumble.x = 0.1f;
		}


		//pause
		if (state.buttons.menu)
		{
			//pause
			rumble.x = 0.1f;
		}
		//exit(debug build)
		if (state.buttons.back)
		{
			//exit game
			rumble.x = 0.1f;
		}
		//vibration 
		//for (int i = 0; i < 4; i++)
		if (rumbleTime>rumbleClock)
		{
			rumbleClock += deltaTime;
			theGamePad.SetVibration(0, rumble.x, rumble.y);
		}
		else
		{
			rumbleClock = 0.f;
			rumbleTime = 0.f;
		}
		
		//on landing
		//on hit with attack
		//on got hit from

		//camera Zoom between player

	}
}

float InputHandler::calcCamera(DirectX::XMFLOAT3 posLeft, DirectX::XMFLOAT3 posRight)
{
	//move this to camera class
	float length = posRight.x - posLeft.x;
	if (length < -2.5f)
		return length;
	else
		return -2.5f;
}

void InputHandler::inPauseMode(int id)
{
	//only one player pauses and controlls the screen at a time
 

	//move camera
	float stickAbsL = abs(this->theGamePad.GetState(id).thumbSticks.leftX);
	if (stickAbsL > 0.f)
	{
		float dir = this->theGamePad.GetState(id).thumbSticks.leftX / stickAbsL;
		//this->obj->move(dir * deltaTime, 0, 0);
	}
	else if (this->theGamePad.GetState(id).dpad.right || this->theGamePad.GetState(id).dpad.left)
	{
		//this->obj->move((state.dpad.right - state.dpad.left) * deltaTime, 0, 0);
	}

	//rotate camera
	float rightStickX = this->theGamePad.GetState(id).thumbSticks.rightX;
	float rightStickY = this->theGamePad.GetState(id).thumbSticks.rightY;
	this->cameraRotation.x += -rightStickY;//* deltaTime;
	this->cameraRotation.y += rightStickX;//* deltaTime;

	//move selection

	//zoom
	if (theGamePad.GetState(id).buttons.x|| theGamePad.GetState(id).buttons.y)
	{
		//cameraPos

		//this->obj->move((state.dpad.right - state.dpad.left) * deltaTime, 0, 0);
	}

	//unpause
	if (this->theGamePad.GetState(id).buttons.menu || theGamePad.GetState(id).buttons.back)
	{

	}

	//L+R+A+Start
	if (this->checkReset(id))
	{
		theGamePad.SetVibration(0, 1.f, 1.f);
	}
}

void InputHandler::inSelectionMode()
{
	//log in player
	//select character


	//right/left- select
	//confirm
	if (theGamePad.GetState(0).buttons.a|| theGamePad.GetState(0).buttons.menu)
	{

	}

	//back	
	if (theGamePad.GetState(0).buttons.b)
	{

	}
	//L+R+A+Start
	//this->checkReset(id);
}

void InputHandler::inMenuMode()
{
	//select/comfirm
	//move
	//rotate menu?
	//back
	//vibration
	//L+R+A+Start
}

InputHandler::InputHandler()
{
	this->rumbleClock = 0.f;
	this->rumbleTime = 0.f;
	this->rumble = { 0.f,0.f };
	this->isJumping = false;
	this->jumpTimer = 0.f;
	for (int i = 0; i < 4; i++)
	{
		player[i].id = i;
		player[i].Active = false;
	}
	
}

InputHandler::~InputHandler()
{
}

DirectX::XMFLOAT3 InputHandler::getDirection(int player)
{
	return direction[player];
}

bool InputHandler::controllerIsConnected(int controllerPort)
{
	return theGamePad.GetState(controllerPort).IsConnected();
}


