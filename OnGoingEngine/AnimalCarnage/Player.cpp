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
	
	//this->playerObj->getRigidbody() = System::getphysices()->addSphere(0.5f,btVector3(0,0,0),1);
	this->playerObj->getRigidbody() = System::getphysices()->addBox(btVector3(0,0,0),btVector3(1,1,1),10.0f);

	this->playerObj->getRigidbody()->setWorldTransform(XMMATRIX_to_btTransform(this->playerObj->getWorld()));
	System::theModelLoader->loadGO(this->playerObj, "Resources/Models/fox_test.lu", "fox_character_diffuse.tga");
	//this->playerObj->setScale(0.4f, 0.2f, 1.0f);
	System::handler->addObject(this->playerObj);

	/////////////
	this->playerObj->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
	this->playerObj->setRotation(0,1,0,3.14/2);
}

void Player::update(float deltaTime, int id)
{
	this->playerObj->setPosition(this->playerObj->getRigidbody()->getWorldTransform().getOrigin().getX()
		, this->playerObj->getRigidbody()->getWorldTransform().getOrigin().getY(), this->playerObj->getRigidbody()->getWorldTransform().getOrigin().getZ());

	//this->playerObj->setRotation(0, 1,this->playerObj->getRigidbody()->getWorldTransform().getRotation().getZ(), 3.14 / 2);

	//kolla collision mellan spelar objecten så de inte rör sig i z-led
	//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(0, 0, 0));
	this->playerObj->getRigidbody()->setLinearFactor(btVector3(1, 1, 0));
	//this->playerObj->getRigidbody()->setAngularFactor(btVector3(1, 0, 0));

	DirectX::GamePad::State state = System::theGamePad->GetState(id);
	if (state.IsConnected())
	{
		System::theTracker->Update(state);
		//Actions 
		if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)		{
			theRumble.rumble.x = 0.6f;
			theRumble.rumble.y = 0.6f;
			theRumble.rumbleTime = 0.3f;
		}
		else if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::HELD){
			theRumble.rumble.x = 0.2f;
			theRumble.rumble.y = 0.2f;
			theRumble.rumbleTime = 0.2f;
		}

		float stickAbsL = abs(state.thumbSticks.leftX);
		if (stickAbsL > 0.f && grounded){
			float dir = 500.0f * state.thumbSticks.leftX;// / stickAbsL;
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(dir, 0, 0));
			if ((playerObj->getRigidbody()->getLinearVelocity().getX() < 5.0f) && (playerObj->getRigidbody()->getLinearVelocity().getX() > -5.0f)) {
				playerObj->getRigidbody()->applyForce(btVector3(dir, 0, 0), btVector3(0, 0, 0));
			}
			airSpeed = dir;

		}
		if ((state.buttons.x || state.buttons.y) && canJump){
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(0,1, 0));
			//his->playerObj->getRigidbody()->applyImpulse(btVector3(0,05.0f,0),btVector3(0,0,0));
			playerObj->getRigidbody()->applyForce(btVector3(0, 500.0f, 0), btVector3(0, 0, 0));
		}
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
