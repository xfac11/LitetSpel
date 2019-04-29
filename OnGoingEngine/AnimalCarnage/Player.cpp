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
	this->playerObj = nullptr;
}



Player::~Player()
{
	playerObj = nullptr;
}

void Player::initialize()
{
	this->playerObj = new GameObject(System::shaderManager->getForwardShader());
	
	this->playerObj->setPosition(0, 1, 1);
	btVector3 postion = btVector3(playerObj->getPosition().x, playerObj->getPosition().y, playerObj->getPosition().z);
	this->playerObj->getRigidbody() = System::getphysices()->addSphere(0.5f, postion.getX(), postion.getX(), postion.getX(),1);
	//this->playerObj->body()->getWorldTransform()
	////btMotionState* ms = this->playerObj->body()->getMotionState();

	////btMatrix3x3 orn = ms->getWorldTransform(); //get basis of world transformation
	////orn *= btMatrix3x3(btQuaternion(axis, radians));     //Multiply it by rotation matrix
	//btMatrix3x3 transform = this->playerObj->body()->getWorldTransform().getBasis();
	//this->playerObj->body()->getWorldTransform().setBasis(transform); //set new rotation for the object
	//this->playerObj->getWorld() = btMatrix3x3(this->playerObj->body()->getInvInertiaTensorWorld());

	this->playerObj->getRigidbody()->setWorldTransform(XMMATRIX_to_btTransform(this->playerObj->getWorld()));
	System::theModelLoader->loadGO(this->playerObj, "Resources/Models/fox_test.lu", "fox_character_diffuse.tga");
	//this->playerObj->setScale(1f, 0.4f, 0.1f);
	System::handler.addObject(this->playerObj);

	/////////////
	this->playerObj->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
}

void Player::update(float deltaTime, int id)
{
	this->playerObj->setPosition(this->playerObj->getRigidbody()->getWorldTransform().getOrigin().getX()
		, this->playerObj->getRigidbody()->getWorldTransform().getOrigin().getY(),
		this->playerObj->getRigidbody()->getWorldTransform().getOrigin().getZ());
	//this->playerObj->body()->setWorldTransform(XMMATRIX_to_btTransform(this->playerObj->getWorld()));
	

	/*this->playerObj->setRotation(this->playerObj->body()->getWorldTransform().getBasis().getgetX()
		, this->playerObj->body()->getWorldTransform().getOrigin().getY(),
		this->playerObj->body()->getWorldTransform().getOrigin().getZ());*/
	/*if (System::theKeyboard->KeyIsPressed('K'))
	{
		this->playerObj->setPosition(0, 100, 1);
	
		this->playerObj->body()->setWorldTransform(XMMATRIX_to_btTransform(this->playerObj->getWorld()));
	}
	*/
	this->playerObj->getRigidbody()->setLinearVelocity(btVector3(0, 0, 0));
	DirectX::GamePad::State state = System::theGamePad->GetState(id);
	if (state.IsConnected())
	{
		System::theTracker->Update(state);
		//Actions 
		if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED) //and
		{
			theRumble.rumble.x = 0.6f;
			theRumble.rumble.y = 0.6f;
			theRumble.rumbleTime = 0.3f;
		}
		if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::HELD) //and
		{
			theRumble.rumble.x = 0.2f;
			theRumble.rumble.y = 0.2f;
			theRumble.rumbleTime = 0.2f;
		}

		//			//GROUND MOVEMENT
		float stickAbsL = abs(state.thumbSticks.leftX);
		if (stickAbsL > 0.f && grounded)
		{
			float dir = 0.5f * state.thumbSticks.leftX;// / stickAbsL;
		//	this->playerObj->getRigidbody()->applyForce(btVector3(dir, 0, 0), btVector3(0, 1, 0));
			this->playerObj->getRigidbody()->setLinearVelocity(btVector3(dir, 0, 0));
			airSpeed = dir;
		}
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
		//JUMP INPUT
		if ((state.buttons.x || state.buttons.y) && canJump)//== DirectX::GamePad::ButtonStateTracker::PRESSED 
		{
			//isJumping = true;
			//grounded = false;
			//this->playerObj->body()->applyImpulse(btVector3(0, 1, 0), btVector3(0, 1, 0));
		}
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



	//		//GROUND MOVEMENT
	//	float stickAbsL = abs(state.thumbSticks.leftX);
	//	if (stickAbsL > 0.f && grounded)
	//	{
	//		float dir = 2 * state.thumbSticks.leftX;// / stickAbsL;

	//		playerObj->move(dir * deltaTime, 0, 0);
	//	}
	//	else if ((state.dpad.right || state.dpad.left) && grounded)
	//	{
	//		float dir = 2 * (state.dpad.right - state.dpad.left);
	//		playerObj->move(dir * deltaTime, 0, 0);
	//	}
	//	else
	//		playerObj->move(0, 0, 0);
	//
	//	//IN AIR MOVEMENT

	//	//else if (isJumping == false)
	//	//	playerObj->move(0, 0, 0);
	//	//else
	//	//	playerObj->move(0, 0, 0);
	//	//move(getPosition().x, 0, getPosition().z);; //= { 0,0,0 };

	////IN AIR MOVEMENT
	//	float airspeedAbs = abs(airSpeed);
	//	if (stickAbsL > 0.f && !grounded)
	//	{
	//		float dir = 0.5f * state.thumbSticks.leftX;// airSpeed;// / stickAbsL;
	//		playerObj->move(5*dir * deltaTime, 0, 0);
	//	}
	//	else if (state.dpad.right && !grounded || state.dpad.left && !grounded)
	//	{
	//		float dir = 0.5f * (state.dpad.right - state.dpad.left);
	//		playerObj->move(5*dir* deltaTime, 0, 0);

	//	}

	//	//JUMP INPUT
	//	if ((state.buttons.x || state.buttons.y) && canJump)
	//	{
	//		isJumping = true;
	//		grounded = false;
	//	}

	//	//canJump
	//	if (state.buttons.x || state.buttons.y) {
	//		canJump = false;
	//	}
	//	if (grounded /*&& System::theTracker->x == DirectX::GamePad::ButtonStateTracker::RELEASED || System::theTracker->y == DirectX::GamePad::ButtonStateTracker::RELEASED*/) {
	//		canJump = true;
	//	}



	//	//falling/jump function
	//	if (grounded == false)
	//	{
	//		airTimer += deltaTime;
	//		playerObj->move(0, (-9.82f * airTimer + (5.f * isJumping)) * deltaTime, 0);
	//		//continue here
	//	}
	//	else if (grounded == true)
	//	{
	//		airTimer = 0.f;
	//		isJumping = false;
	//		this->playerObj->setPosition(this->playerObj->getPosition().x, 0, this->playerObj->getPosition().z);
	//		airSpeed = 0.0f;
	//	}
	//	if (this->playerObj->getPosition().y < 0) {
	//		grounded = true;
	//	}



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
			theRumble.rumble.x = 0.4f;
			theRumble.rumble.y = 0.4f;
			theRumble.rumbleTime = 0.4f;
		}

		//pause
		if (state.buttons.menu)
		{
			//pause
			theRumble.rumble.x = 0.3f;
			theRumble.rumbleTime = 0.1f;
		}

		//exit(debug build)
		if (state.buttons.back)
		{
			//exit game
			theRumble.rumble.y = 0.3f;
			theRumble.rumbleTime = 0.1f;
		}
	}
}

bool Player::updateRumble(float dt, int id)
{
	if (!theRumble.rumbleEnabled) {
		return false;
	}
	if (theRumble.rumbleTime > theRumble.rumbleClock)
	{
		theRumble.rumbleClock += dt;
		System::theGamePad->SetVibration(id, theRumble.rumble.x, theRumble.rumble.y);
	}
	else
	{
		theRumble.rumbleClock = 0.f;
		theRumble.rumbleTime = 0.f;
		System::theGamePad->SetVibration(id, 0, 0);
	}

	return true;
}

bool Player::setRumble(bool rumble)
{
	return this->theRumble.rumbleEnabled = rumble;
}






XMFLOAT3 Player::mul(XMFLOAT3 l, float r)
{
	return XMFLOAT3(l.x * r, l.y * r, l.z * r);
}

XMFLOAT3 Player::add(XMFLOAT3 l, XMFLOAT3 r)
{
	return XMFLOAT3(l.x + r.x, l.y + r.y, l.z + r.z);
}


void Player::addForce(XMFLOAT3 Dir, float force)
{
	this->Velocity = XMFLOAT3(0, 0, 0);
	this->Accleration = mul(Dir, force);
}


void Player::move(float x, float y, float z)
{
	this->playerObj->move(x, y, z);
}

void Player::move(XMFLOAT3 source) //fix this ALAN
{
	//this->move(source);
}

void Player::setPosition(float x, float y, float z)
{
	this->playerObj->setPosition(x, y, z);
}

void Player::setScale(float x, float y, float z)
{
	this->playerObj->setScale(x, y, z);
}

XMFLOAT3 Player::getPosition()
{
	return this->playerObj->getPosition();
}

AABB Player::getAABB()
{
	return this->playerObj->getCollisionBox();
}

float Player::magnitude(XMFLOAT3 l)
{
	float temp = sqrt((l.x * l.x) + (l.y * l.y) + (l.z * l.z));
	return temp;
}

XMFLOAT3 Player::normalize(XMFLOAT3 l)
{
	float length = this->magnitude(l);
	if (length != 0)
		return XMFLOAT3(l.x / length, l.y / length, l.z / length);
	else
		return l;
}

float Player::dot(const XMFLOAT3 & l, const XMFLOAT3 & r)
{
	return (l.x * r.x) + (l.y * r.y) + (l.z * r.z);
}

XMFLOAT3 Player::cross(const XMFLOAT3 & l, XMFLOAT3 & r)
{

	return XMFLOAT3(l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x);
}
