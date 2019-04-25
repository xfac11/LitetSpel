#include "Player.h"
#include "System.h"


Player::Player()
{
	isJumping = false;
	canJump = true;
	airTimer = 0.f;
	airSpeed = 0.f;
	grounded = true;
	jumpDir = 0;
	inAir = false;
}



Player::~Player()
{
	//delete playerObj;
	
	playerObj = nullptr;
}

void Player::initialize()
{
	this->playerObj = new GameObject(System::shaderManager->getForwardShader());
	
	System::theModelLoader->loadGO(this->playerObj, "Resources/Models/cube2.lu", "cat2.tga");
	this->playerObj->setScale(0.5f, 0.4f, 0.1f);

	System::handler.addObject(this->playerObj);
}

void Player::update(float deltaTime, int id)
{

	DirectX::GamePad::State state = System::theGamePad->GetState(id);
	if (state.IsConnected())
	{
		System::theTracker->Update(state);
		//Actions 

		if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED) //and
		{
			System::theRumble->rumble.x = 0.6f;
			System::theRumble->rumble.y = 0.6f;
			System::theRumble->rumbleTime = 0.3f;
		}
		if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::HELD) //and
		{
			System::theRumble->rumble.x = 0.2f;
			System::theRumble->rumble.y = 0.2f;
			System::theRumble->rumbleTime = 0.2f;
		}
		//if (System::theTracker->dpadUp == DirectX::GamePad::ButtonStateTracker::RELEASED) //and
		//{
		//	System::theRumble[i].rumble.x = 0.2f;
		//	System::theRumble[i].rumble.y = 0.2f;
		//	System::theRumble[i].rumbleTime = 0.2f;
		//}


		//			//GROUND MOVEMENT
		//float stickAbsL = abs(state.thumbSticks.leftX);
		//if (stickAbsL > 0.f && grounded)
		//{
		//	float dir = 5 * state.thumbSticks.leftX;// / stickAbsL;
		//	move(dir * deltaTime, 0, 0);
		//	airSpeed = dir;
		//}
		//else if ((state.dpad.right || state.dpad.left) && grounded)
		//{
		//	move(5 * (state.dpad.right - state.dpad.left) * deltaTime, 0, 0);
		//	airSpeed = 5.f * (state.dpad.right - state.dpad.left);
		//}
		//else if (isJumping == false)
		//	move(0,0,0);
		//else
		//	move(0, 0, 0);
		////IN AIR MOVEMENT
		//float airspeedAbs = abs(airSpeed);
		//if (stickAbsL > 0.f && !grounded)
		//{
		//	airSpeed += 0.5f * state.thumbSticks.leftX;
		//	float dir = airSpeed;// / stickAbsL;
		//	move(dir * deltaTime, 0, 0);
		//	if (airspeedAbs > 5)
		//		airSpeed = airspeedAbs / airSpeed * 5;
		//}
		//else if (state.dpad.right && !grounded || state.dpad.left && !grounded)
		//{
		//	airSpeed += 0.5f * (state.dpad.right - state.dpad.left);
		//	if (!grounded) {
		//		move(airSpeed * deltaTime, 0, 0);
		//	}
		//	if (airspeedAbs > 5)
		//		airSpeed = airspeedAbs / airSpeed * 5;
		//}
		////JUMP INPUT
		//if ((state.buttons.x || state.buttons.y) && canJump)//== DirectX::GamePad::ButtonStateTracker::PRESSED 
		//{
		//	isJumping = true;
		//	grounded = false;
		//}
		////canJump
		//if (state.buttons.x || state.buttons.y) {
		//	canJump = false;
		//}
		//if (grounded /*&& System::theTracker->x == DirectX::GamePad::ButtonStateTracker::RELEASED || System::theTracker->y == DirectX::GamePad::ButtonStateTracker::RELEASED*/) {
		//	canJump = true;
		//}
		////falling/jump function
		//if (grounded == false)
		//{
		//	airTimer += deltaTime;
		//	move(0,-9.82f * airTimer + (5.f * isJumping) * deltaTime,0);
		//	//continue here
		//}
		//else if (grounded == true)
		//{
		//	airTimer = 0.f;
		//	isJumping = false;
		//	setPosition(player->getPosition().x, 0, player->getPosition().z); //= { 0,0,0 };
		//	airSpeed = 0.0f;
		//}



			//GROUND MOVEMENT
		float stickAbsL = abs(state.thumbSticks.leftX);
		if (stickAbsL > 0.f && grounded)
		{
			float dir = 5 * state.thumbSticks.leftX;// / stickAbsL;

			playerObj->move(dir * deltaTime, 0, 0);
			airSpeed = dir;
		}
		else if ((state.dpad.right || state.dpad.left) && grounded)
		{
			playerObj->move(5 * (state.dpad.right - state.dpad.left) * deltaTime, 0, 0);
			airSpeed = 5.f * (state.dpad.right - state.dpad.left);
		}
		//else if (isJumping == false)
		//	playerObj->move(0, 0, 0);
		//else
		//	playerObj->move(0, 0, 0);
		//move(getPosition().x, 0, getPosition().z);; //= { 0,0,0 };

	//IN AIR MOVEMENT
		float airspeedAbs = abs(airSpeed);
		if (stickAbsL > 0.f && !grounded)
		{
			airSpeed += 0.5f * state.thumbSticks.leftX;
			float dir = airSpeed;// / stickAbsL;

			playerObj->move(dir * deltaTime, 0, 0);

			if (airspeedAbs > 5)
				airSpeed = airspeedAbs / airSpeed * 5;
		}
		else if (state.dpad.right && !grounded || state.dpad.left && !grounded)
		{
			airSpeed += 0.5f * (state.dpad.right - state.dpad.left);

			if (!grounded) {
				playerObj->move(airSpeed * deltaTime, 0, 0);
			}

			if (airspeedAbs > 5)
				airSpeed = airspeedAbs / airSpeed * 5;
		}



		//JUMP INPUT
		if ((state.buttons.x || state.buttons.y) && canJump)
		{
			isJumping = true;
			grounded = false;
		}

		//canJump
		if (state.buttons.x || state.buttons.y) {
			canJump = false;
		}
		if (grounded /*&& System::theTracker->x == DirectX::GamePad::ButtonStateTracker::RELEASED || System::theTracker->y == DirectX::GamePad::ButtonStateTracker::RELEASED*/) {
			canJump = true;
		}



		//falling/jump function
		if (grounded == false)
		{
			airTimer += deltaTime;
			playerObj->move(0, (-9.82f * airTimer + (5.f * isJumping)) * deltaTime, 0);
			//continue here
		}
		else if (grounded == true)
		{
			airTimer = 0.f;
			isJumping = false;
			this->playerObj->setPosition(this->playerObj->getPosition().x, 0, this->playerObj->getPosition().z);
			airSpeed = 0.0f;
		}
		if (this->playerObj->getPosition().y < 0) {
			grounded = true;
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


		if (state.buttons.leftShoulder ||
			state.buttons.rightShoulder)
		{
			System::theRumble->rumble.x = 0.4f;
			System::theRumble->rumble.y = 0.4f;
			System::theRumble->rumbleTime = 0.4f;
		}

		//pause
		if (state.buttons.menu)
		{
			//pause
			System::theRumble->rumble.x = 0.3f;
			System::theRumble->rumbleTime = 0.1f;
		}

		//exit(debug build)
		if (state.buttons.back)
		{
			//exit game
			System::theRumble->rumble.y = 0.3f;
			System::theRumble->rumbleTime = 0.1f;
		}

		//this->updateRumble(deltaTime, i);
	//}

	}
}

