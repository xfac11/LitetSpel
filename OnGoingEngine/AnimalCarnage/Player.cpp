#include "Player.h"
#include "System.h"

float Player::getJumpHeight() const
{
	return Animal::getAnimal(this->type).jumpHeight;
}

int Player::getStrength() const
{
	return Animal::getAnimal(this->type).strength;
}

float Player::getSpeed() const
{
	return Animal::getAnimal(this->type).speed;
}

float Player::getWeight() const
{
	return Animal::getAnimal(this->type).weight;
}

int Player::getMaxHealth() const
{
	return Animal::getAnimal(this->type).maxHealh;
}

int Player::getHealth() const
{
	return this->health;
}

void Player::takeDamage(int damage)
{
	this->health -= damage;
	
	if (this->health < 0)
	{
		this->health = 0;
	}
}

bool Player::isDead() const
{
	return this->health <= 0;
}

void Player::changeCharacter()
{
	if (/*!this->isDead() &&*/ currentAnimal == 3) {
		
		System::setState(MAINMENU);
		return;
	}
	currentAnimal++;
	const AnimalDef& animal = Animal::getAnimal(ArrayOfAnimals[currentAnimal]);
	this->type = ArrayOfAnimals[currentAnimal];
	this->health = animal.maxHealh;

	btVector3 inertia(0, 0, 0);
	//playerObj->getRigidbody()->getCollisionShape()->calculateLocalInertia(getWeight(), inertia);
	playerObj->getRigidbody()->setMassProps(10*getWeight(), inertia);
	//TEMP CHANGE MODEL
	//System::theModelLoader->loadGO(this->playerObj, animal.modelPath);
}

bool Player::getHitStun()
{
	return this->hitStun;
}

void Player::setHitStun(bool hitStun)
{
	this->hitStun = hitStun;
}

Player::Player()
{
	isJumping = false;
	canJump = true;
	canPressJump = true;
	airTimer = 0.f;
	airSpeed = 0.f;
	grounded = true;
	jumpDir = 0;
	inAir = false;
	dir = 1;
	this->playerObj = nullptr;
	facing = 0.0f;
	canWallJump = false;
	wallJumpReset = false;
	hitStun = false;
	hitTime = 100;
	hitTime2 = 100;
	type = DEFAULT_TYPE;
	health = 100;
	canPressPunch = true;
	deathTimer = 0;
	groundTimer = 0;
	
	currentAnimal = 0;
	ArrayOfAnimals[0] = FOX;
	ArrayOfAnimals[1] = BEAR;
	ArrayOfAnimals[2] = RABBIT;
	ArrayOfAnimals[3] = MOOSE;

}



Player::~Player()
{
	playerObj = nullptr;
	//delete this->hitbox.hitbox;
}

void Player::initialize(AnimalType type)
{
	//setup health
	const AnimalDef& animal = Animal::getAnimal(type);
	this->type = type;
	this->health = animal.maxHealh;

	this->playerObj = new GameObject(System::shaderManager->getForwardShader());
	this->hitbox.hitbox = new GameObject();
	//this->hitbox.hitbox->setRotation(0, 1, 0, 3.14 / 2);
	this->hitbox.time = 0;
	this->hitbox.totalTime = 30;
	System::theModelLoader->loadGO(this->hitbox.hitbox, "Resources/Models/cube2.lu");
	this->hitbox.hitbox->setScale(0.2f, 0.2f, 0.2f);
	System::handler->addObject(this->hitbox.hitbox);
	//this->playerObj->getRigidbody() = System::getphysices()->addSphere(0.5f,btVector3(0,0,0),1);

	//load player
	//AABB aabb = playerObj->getCollisionBox();
	//btVector3 size = btVector3(1 + aabb.width * 2, aabb.height * 2, 1);
	//playerObj->CollisionShape->Initialize(1, btVector3(0, 0, 0), size);
	////DirectX::XMMatrixTranslation(this->Position.x+posOffset[0])
	//playerObj->CollisionShape->SetWorld(&playerObj->getWorld());
	//System::getDebugDraw()->addPrimitives(playerObj->CollisionShape);

	System::theModelLoader->loadGO(this->playerObj, animal.modelPath);
	System::handler->addObject(this->playerObj);


	AABB aabb = playerObj->getCollisionBox();
	btVector3 size = btVector3(1+aabb.width*2, aabb.height*2,1);
	playerObj->getRigidbody() = System::getphysices()->addPlayer(btVector3(0, 0, 0), size, 10.0f * getWeight(),this);
	//this->playerObj->getRigidbody()->getWorldTransform().setRotation(btQuaternion(3.14 / 2, 0, 0));
	playerObj->getRigidbody()->setWorldTransform(XMMATRIX_to_btTransform(this->playerObj->getWorld()));
	this->playerObj->setRotationRollPitchYaw(0.f,3.14f/2.f,0.f);;


	//System::handler->addObject(this->hitbox.hitbox);
	Primitives *CollisionShape;
	CollisionShape = new Primitives();
	CollisionShape->Initialize(1, btVector3(0,0,0), btVector3(0, 0, 0));
	/*DirectX::XMMatrixTranspose(
		XMMatrixScaling(Scale.x, Scale.y, Scale.z) *
		XMMatrixTranslation(Position.x, Position.y, Position.z));*/
	CollisionShape->SetWorld(&this->hitbox.hitbox->getWorld());
	System::getDebugDraw()->addPrimitives(CollisionShape);
	/////////////
	playerObj->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
	playerObj->getRigidbody()->setFriction(0.5);
	playerObj->getRigidbody()->setRestitution(0);
	playerObj->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));


}

void Player::update(float deltaTime, int id)
{
	//check if player is dead and have any animals left to play
	//changeCharacter();
	string str = to_string(currentAnimal) + "\n";
	OutputDebugString( str.c_str() );

	//Cool rotation
	this->playerObj->setRotationRollPitchYaw(-(this->playerObj->getRigidbody()->getLinearVelocity().getY() / 20), this->playerObj->getRotation().y, this->playerObj->getRotation().z);
	if (this->playerObj->getRotation().x > 0.785398163) {
		this->playerObj->setRotationRollPitchYaw(0.785398163, this->playerObj->getRotation().y, this->playerObj->getRotation().z);
	}
	if (this->playerObj->getRotation().x < -0.785398163) {
		this->playerObj->setRotationRollPitchYaw(-0.785398163, this->playerObj->getRotation().y, this->playerObj->getRotation().z);
	}

	//Hitstun
	if (this->hitStun == true) {
		hitTime -= 165 * deltaTime;
		string str = to_string(hitTime) + "\n";
		if (dir = 1) {
			this->playerObj->setRotationRollPitchYaw(this->playerObj->getRotation().x + (hitTime * 0.007), this->playerObj->getRotation().y, this->playerObj->getRotation().z);
		}
		if (dir = -1) {
			this->playerObj->setRotationRollPitchYaw(this->playerObj->getRotation().x - (hitTime * 0.007)*2, this->playerObj->getRotation().y, this->playerObj->getRotation().z);
		}

	}
	if (hitTime <= 0) {
		hitTime = 100;
		hitStun = false;
	}

	this->playerObj->setPosition(this->playerObj->getRigidbody()->getWorldTransform().getOrigin().getX()
		, this->playerObj->getRigidbody()->getWorldTransform().getOrigin().getY(), this->playerObj->getRigidbody()->getWorldTransform().getOrigin().getZ());

	//this->playerObj->setRotation(0, 1,this->playerObj->getRigidbody()->getWorldTransform().getRotation().getZ(), 3.14 / 2);
	
	/*playerObj->setRotation(this->playerObj->getRigidbody()->getWorldTransform().getRotation().getAxis().getX(),
		this->playerObj->getRigidbody()->getWorldTransform().getRotation().getAxis().getY(),
		this->playerObj->getRigidbody()->getWorldTransform().getRotation().getAxis().getZ(),
		this->playerObj->getRigidbody()->getWorldTransform().getRotation().getAngle());*/

	//kolla collision mellan spelar objecten så de inte rör sig i z-led
	//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(0, 0, 0));
	this->playerObj->getRigidbody()->setLinearFactor(btVector3(1, 1, 0));
	this->playerObj->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));
	//btVector3 velocity = this->playerObj->getRigidbody()->getLinearVelocity();
	//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(velocity.getX(), 0, velocity.getZ()));



	if (hitbox.time == 0)
	{
		this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
	}
	this->hitbox.hitbox->setPosition(this->hitbox.hitbox->getPosition().x, this->getPosition().y, this->getPosition().z);
	DirectX::GamePad::State state = System::theGamePad->GetState(id);
	if (state.IsConnected() && !hitStun && !(getHealth() == 0 || getHealth() < 0))
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
		
		//GROUND MOVEMENT
		float stickAbsL = abs(state.thumbSticks.leftX);
		if (stickAbsL > 0.f && canJump  && this->hitbox.time == 0){
			float dir = 17.0f * state.thumbSticks.leftX  * getSpeed();// / stickAbsL;
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(dir, 0, 0));
			
			playerObj->getRigidbody()->setLinearVelocity(btVector3(dir, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));

			if (playerObj->getRigidbody()->getLinearVelocity().getX() > 17.0f * getSpeed()) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(17.0f * getSpeed(), playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			}
			if (playerObj->getRigidbody()->getLinearVelocity().getX() < -17.0f * getSpeed()) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(-17.0f * getSpeed(), playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			}
			airSpeed = dir;
		}

		//AIR MOVEMENT && //FAST FALL
		if (stickAbsL > 0.f && !canJump) {
			float dir = 400.0f * state.thumbSticks.leftX  * getSpeed() * getWeight();// / stickAbsL;
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(dir, 0, 0));

			playerObj->getRigidbody()->applyForce(btVector3(dir, 0, 0), btVector3(0, 0, 0));

			if (playerObj->getRigidbody()->getLinearVelocity().getX() > 17.0f* getSpeed()) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(17.0f* getSpeed(), playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			}
			if (playerObj->getRigidbody()->getLinearVelocity().getX() < -17.0f* getSpeed()) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(-17.0f* getSpeed(), playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			}
			airSpeed = dir;

		}
		if (state.thumbSticks.leftY < 0 && !canJump) {
			if (state.thumbSticks.leftY < -0.8) {
				playerObj->getRigidbody()->applyForce(btVector3(0, state.thumbSticks.leftY * 1700.0f, 0), btVector3(0, 0, 0));
				if (playerObj->getRigidbody()->getLinearVelocity().getY() < -30.0f) {
					playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX(), -30.0f, playerObj->getRigidbody()->getLinearVelocity().getZ()));

				}	
			}
			//playerObj->getRigidbody()->applyForce(btVector3(0, state.thumbSticks.leftY * 700, 0), btVector3(0, 0, 0));
		}
		

		//JUMP
		if ((state.buttons.x) && canJump && canPressJump){
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(0,1, 0));
			this->playerObj->getRigidbody()->applyImpulse(btVector3(0, 205.0f * getJumpHeight() * getWeight() /**deltaTime * 60*/,0),btVector3(0,0,0));
			//playerObj->getRigidbody()->applyForce(btVector3(0, 4500.0f, 0), btVector3(0, 0, 0));
			grounded = false;
			canWallJump = false;
			canPressJump = false;
		}
		if (grounded == true) {
			wallJumpReset = true;
			canJump = true;
		}
		else if (grounded == false) {
			canJump = false;
		}


		//WALLJUMP
		if ((state.buttons.x) && canWallJump && !canJump && !grounded && wallJumpReset && canPressJump && type == FOX) {
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(0,1, 0));
			this->playerObj->getRigidbody()->applyImpulse(btVector3(0, 225.0f * getJumpHeight() * getWeight() /**deltaTime * 60*/, 0), btVector3(0, 0, 0));
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX()*-1, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			//playerObj->getRigidbody()->applyForce(btVector3(0, 4500.0f, 0), btVector3(0, 0, 0));
			grounded = false;
			canWallJump = false;
			wallJumpReset = false;

			float dir = 17.0f * state.thumbSticks.leftX;// / stickAbsL;
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(dir, 0, 0));

			playerObj->getRigidbody()->setLinearVelocity(btVector3(dir, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));


		}
		
		if ((state.buttons.x)) {
			canPressJump = false;
		}
		else {
			canPressJump = true;
		}

		//PUNCH
		if (hitbox.time > 0 && hitbox.time < hitbox.totalTime)
		{
			hitbox.time += 1 * deltaTime * 60;
		}
		if (hitbox.time >= hitbox.totalTime)
		{
			this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
			this->hitbox.time = 0;
		}
		float dist = 700;
		if ((state.buttons.y && canPressPunch) || (this->hitbox.time > 0))
		{

			/*if (this->hitbox.time > 0 &&state.thumbSticks.leftX < 0)
			{
				dist = -700;
			}
			else if (this->hitbox.time > 0 && state.thumbSticks.leftX > 0)
			{
				dist = 700;
			}*/
			if (grounded) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX() / 1.1, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
			}

			if (hitbox.time > hitbox.totalTime / 2)
				this->hitbox.hitbox->move((dir*-dist * deltaTime) / hitbox.totalTime, 0, 0);
			else
			{
				this->hitbox.hitbox->move((dir*dist * deltaTime) / hitbox.totalTime, 0, 0);
			}
			this->hitbox.time++;
		}

		//BEAR ATTACK
		/*if (hitbox.time > 0 && hitbox.time < hitbox.totalTime)
		{
			hitbox.time += 1 * deltaTime * 60;
		}
		if (hitbox.time >= hitbox.totalTime)
		{
			this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
			this->hitbox.time = 0;
		}
		dist = 700;
		if (((state.buttons.a && canPressPunch) || (this->hitbox.time > 0)) && (type == BEAR))
		{
			if (grounded) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX() / 1.1, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
			}
			if (hitbox.time > hitbox.totalTime / 10 && hitbox.time < hitbox.totalTime) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(dir*20.0f, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			}

			if (hitbox.time > hitbox.totalTime / 2)
				this->hitbox.hitbox->move((dir*-dist * deltaTime) / hitbox.totalTime, 0, 0);
			else
			{
				this->hitbox.hitbox->move((dir*dist * deltaTime) / hitbox.totalTime, 0, 0);
			}
			this->hitbox.time++;
		}*/

		if ((state.buttons.y) || (state.buttons.a)) {
				canPressPunch = false;
		}
		else {
				canPressPunch = true;
		}

		if (state.buttons.leftShoulder ||
			state.buttons.rightShoulder)
		{
			theRumble.rumble.x = 0.4f;
			theRumble.rumble.y = 0.4f;
			theRumble.rumbleTime = 0.4f;
		}


		//Facing Direction
		
		if (dir == 1 && grounded) {
			facing += 0.5f*deltaTime * 60;
		}
		if (dir == -1 && grounded) {
			facing -= 0.5f*deltaTime * 60;
		}
		if (dir == 1 && !grounded) {
			facing += 0.1f*deltaTime * 60;
		}
		if (dir == -1 && !grounded) {
			facing -= 0.1f*deltaTime*60;
		}
		if (state.thumbSticks.leftX < 0 && dir == 1)
		{
			dir = -1;
			this->playerObj->setRotationRollPitchYaw(this->playerObj->getRotation().x, facing, this->playerObj->getRotation().z);
		}
		else if (state.thumbSticks.leftX > 0 && dir == -1)
		{
			dir = 1;
			this->playerObj->setRotationRollPitchYaw(this->playerObj->getRotation().x, facing, this->playerObj->getRotation().z);
		}
		if (facing > (3.14f / 2.f)) {
			facing = 3.14f/ 2.f;
		}
		if (facing < (-3.14f /2.f)) {
			facing = -3.14f / 2.f;
		}
		this->playerObj->setRotationRollPitchYaw(this->playerObj->getRotation().x, facing, this->playerObj->getRotation().z);
	}

	if (isDead()) {
		//this->playerObj->getRigidbody()->setLinearFactor(btVector3(1, 1, 0));
		//this->playerObj->getRigidbody()->setAngularFactor(btVector3(0, 0, 1));

		//this->playerObj->setRotation(this->playerObj->getRigidbody()->getWorldTransform().getRotation().getX(), this->playerObj->getRotation().y, this->playerObj->getRotation().z, 3.14 / 2);
		/*this->playerObj->setRotationRollPitchYaw(this->playerObj->getRigidbody()->getWorldTransform().getRotation().getX() *3.14 * 2,
			this->playerObj->getRigidbody()->getWorldTransform().getRotation().getY() *3.14 * 2,
			this->playerObj->getRigidbody()->getWorldTransform().getRotation().getZ() *3.14 * 2);*/


			//this->playerObj->setRotation(0, 1,this->playerObj->getRigidbody()->getWorldTransform().getRotation().getZ(), 3.14 / 2);
			//this->playerObj->setRotation(this->playerObj->getRigidbody()->getWorldTransform().getRotation().getX(), this->playerObj->getRotation().y, this->playerObj->getRotation().z, this->playerObj->getRigidbody()->getWorldTransform().getRotation().getAngle());
		deathTimer += 65 * deltaTime;
		if (deathTimer >= 100) {
			health = getMaxHealth();
			deathTimer = 0;
			setRigidbodyPosition(0,+20,0);
		}
		
	}

	groundTimer += deltaTime * 1000;
	if (groundTimer >= 100) {
		grounded = false;
		groundTimer = 100;
	}
	//grounded = false;
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
	this->playerObj->move(source);
}

void Player::setPosition(float x, float y, float z)
{
	this->playerObj->setPosition(x, y, z);
}

void Player::setRigidbodyPosition(float x, float y, float z)
{
	this->playerObj->getRigidbody()->getWorldTransform().setOrigin(btVector3(x,y,x));
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

void Player::setGrounded(bool grounded)
{
	this->grounded = grounded;
	this->groundTimer = 0;
	//playerObj->getRigidbody()->applyImpulse(btVector3(0, 100.0f, 0), btVector3(0, 0, 0));
}

void Player::setCanWallJump(bool canWallJump)
{
	this->canWallJump = canWallJump;
}

void Player::addGroundMovingSpeed(XMFLOAT3 speed)
{
	//playerObj->getRigidbody()->setLinearVelocity(btVector3((playerObj->getRigidbody()->getLinearVelocity().getX() + speed.x), playerObj->getRigidbody()->getLinearVelocity().getY() + speed.y, playerObj->getRigidbody()->getLinearVelocity().getZ() + speed.z));
	//playerObj->getRigidbody()->applyCentralImpulse(btVector3((playerObj->getRigidbody()->getLinearVelocity().getX() + speed.x), playerObj->getRigidbody()->getLinearVelocity().getY() + speed.y, playerObj->getRigidbody()->getLinearVelocity().getZ() + speed.z));
	//OutputDebugStringA("0\n");
}

void Player::follow(XMFLOAT3 pos)
{
	btRigidBody* rg = this->playerObj->getRigidbody();
	XMFLOAT3 pos2 = this->playerObj->GetPosition();

	pos2 = VECTORSUBTRACTION(pos, pos2);

	this->playerObj->Move(pos2);
	/*btTransform trans = rg->getWorldTransform();
	trans.setOrigin(btVector3(pos2.x,pos2.y,pos2.z));
	rg->setWorldTransform(trans);*/

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
