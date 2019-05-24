#include "Player.h"
#include "System.h"

float Player::getAnimSpeed() const
{
	return this->animSpeed;
}

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

void Player::setColorMask(PlayerColor color)
{
	this->color = color;
	XMFLOAT4 colorMask;

	switch (color)
	{
	case RED:
		colorMask = XMFLOAT4(Colors::Red);
		break;
	case BLUE:
		colorMask = XMFLOAT4(Colors::Blue);
		break;
	case GREEN:
		colorMask = XMFLOAT4(Colors::Green);
		break;
	case YELLOW:
		colorMask = XMFLOAT4(Colors::Yellow);
		break;
	case CYAN:
		colorMask = XMFLOAT4(Colors::Cyan);
		break;
	case PINK:
		colorMask = XMFLOAT4(Colors::Pink);
		break;
	case BLACK:
		colorMask = XMFLOAT4(Colors::Black);
		break;
	case WHITE:
		colorMask = XMFLOAT4(Colors::White);
		break;
	}

	this->playerObj->setColorMask(colorMask);
}

void Player::setAnimalType(AnimalType type)
{
	const AnimalDef& animal = Animal::getAnimal(type);
	this->type = type;
	this->health = animal.maxHealh;
}

void Player::changeCharacter()
{
	if (canBeAnimal[0] == false && canBeAnimal[1] == false && canBeAnimal[2] == false && canBeAnimal[3] == false) {
		//RESULT SCREEN
		System::setState(MAINMENU);
		return;
	}
	int noNewAnimal = true;
	for (int i = 0; i < 4; i++) 
	{
		if (canBeAnimal[randomNumberArray[i]] == true && noNewAnimal) 
		{
			currentAnimal = randomNumberArray[i];
			canBeAnimal[randomNumberArray[i]] = false;
			noNewAnimal = false;
		}
		if (canBeAnimal[randomNumberArray[i]] == true && !noNewAnimal)
		{
			nextAnimal = randomNumberArray[i];
			break;
		}
		else {
			nextAnimal = -1;
		}



	}

	this->setAnimalType(ArrayOfAnimals[currentAnimal]);
	System::theModelLoader->loadGO(this->playerObj, Animal::getAnimal(ArrayOfAnimals[currentAnimal]).modelPath);
	//if (animal.maskPath != "empty" && !this->playerObj->getModel()->hasMaskColor())
	//	this->playerObj->setMask(animal.maskPath, 0);//change to animal.maskPath
	//System::handler->addObject(this->playerObj);
	


	btVector3 inertia(0, 0, 0);
	playerObj->getRigidbody()->setMassProps(10*getWeight(), inertia);


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
	canDoubleJump = false;
	wallJumpReset = false;
	hitStun = false;
	hitTime = 100;
	hitTime2 = 100;
	hitTimer = 0;
	punching = false;
	type = FOX;
	color = RED;
	health = 100;
	canPressPunch = true;
	deathTimer = 0;
	groundTimer = 0;
	nextAnimal = 0;
	animSpeed = 1;
	
	currentAnimal = 0;
	ArrayOfAnimals[0] = FOX;
	ArrayOfAnimals[1] = BEAR;
	ArrayOfAnimals[2] = RABBIT;
	ArrayOfAnimals[3] = MOOSE;

	for (int i = 0; i < 4; i++) {
		canBeAnimal[i] = true;
	}

}



Player::~Player()
{
	playerObj = nullptr;
	//delete this->hitbox.hitbox;
}

void Player::initialize(AnimalType type, PlayerColor color)
{
	this->setAnimalType(type);

	if (type == FOX)
		canBeAnimal[0] = false;
	if (type == BEAR)
		canBeAnimal[1] = false;
	if (type == RABBIT)
		canBeAnimal[2] = false;
	if (type == MOOSE)
		canBeAnimal[3] = false;

	std::random_shuffle(std::begin(randomNumberArray), std::end(randomNumberArray));


	for (int i = 0; i < 4; i++)
	{
		if (canBeAnimal[randomNumberArray[i]] == true)
		{
			nextAnimal = randomNumberArray[i];
			break;
		}
	}


	this->playerObj = new GameObject(System::shaderManager->getForwardShader());
	this->hitbox.hitbox = new GameObject();
	//this->hitbox.hitbox->setRotation(0, 1, 0, 3.14 / 2);
	this->hitbox.time = 0;
	this->hitbox.totalTime = 30;
	System::theModelLoader->loadGO(this->hitbox.hitbox, "Resources/Models/cube2.lu");
	this->hitbox.hitbox->setScale(0.5f, 0.5f, 0.5f);
	System::handler->addObject(this->hitbox.hitbox);

	//loads animal
	System::theModelLoader->loadGO(this->playerObj, Animal::getAnimal(type).modelPath);
	if(Animal::getAnimal(type).maskPath!="empty"&&!this->playerObj->getModel()->hasMaskColor())
		this->playerObj->setMask(Animal::getAnimal(type).maskPath,0);//change to animal.maskPath
	System::handler->addObject(this->playerObj);

	AABB aabb = playerObj->getCollisionBox();
	btVector3 size = btVector3(1+aabb.width*2, aabb.height*2,1);
	playerObj->getRigidbody() = System::getphysices()->addPlayer(btVector3(0, 0, 0), size, 10.0f * getWeight(),this);

	playerObj->getRigidbody()->setWorldTransform(XMMATRIX_to_btTransform(this->playerObj->getWorld()));
	this->playerObj->setRotationRollPitchYaw(0.f,3.14f/2.f,0.f);;


	Primitives *CollisionShape;
	CollisionShape = new Primitives();
	CollisionShape->Initialize(1, btVector3(0,0,0), btVector3(0, 0, 0));
	CollisionShape->SetWorld(&this->hitbox.hitbox->getWorld());
	System::getDebugDraw()->addPrimitives(CollisionShape);

	playerObj->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
	playerObj->getRigidbody()->setFriction(0.5);
	playerObj->getRigidbody()->setRestitution(0);
	playerObj->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));

	this->setColorMask(color);
}

void Player::update(float deltaTime, int id)
{

	if (type == FOX)
		canBeAnimal[0] = false;
	if (type == BEAR)
		canBeAnimal[1] = false;
	if (type == RABBIT)
		canBeAnimal[2] = false;
	if (type == MOOSE)
		canBeAnimal[3] = false;



	//check if player is dead and have any animals left to play
	//changeCharacter();
	string str = "Player next animal: " + to_string(nextAnimal) + "\n";
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
			this->playerObj->setRotationRollPitchYaw(this->playerObj->getRotation().x + (hitTime * 0.007f), this->playerObj->getRotation().y, this->playerObj->getRotation().z);
		}
		if (dir = -1) {
			this->playerObj->setRotationRollPitchYaw(this->playerObj->getRotation().x - (hitTime * 0.007f)*2, this->playerObj->getRotation().y, this->playerObj->getRotation().z);
		}

	}
	if (hitTime <= 0) {
		hitTime = 100;
		hitStun = false;
	}

	this->playerObj->setPosition(this->playerObj->GetPosition());

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
		if ((stickAbsL > 0.f && canJump  && this->punching == false) && !(punching == true && type == MOOSE)){
			float dir = 17.0f * state.thumbSticks.leftX  * getSpeed();// / stickAbsL;
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(dir, 0, 0));
			
			animSpeed = abs(state.thumbSticks.leftX);

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
		if ((stickAbsL > 0.f && !canJump) && !(punching == true && type == MOOSE)) {
			float dir = 400.0f * state.thumbSticks.leftX  * getSpeed() * getWeight();// / stickAbsL;
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(dir, 0, 0));

			animSpeed = abs(state.thumbSticks.leftX);

			playerObj->getRigidbody()->applyForce(btVector3(dir, 0, 0), btVector3(0, 0, 0));

			if (playerObj->getRigidbody()->getLinearVelocity().getX() > 17.0f* getSpeed()) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(17.0f* getSpeed(), playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			}
			if (playerObj->getRigidbody()->getLinearVelocity().getX() < -17.0f* getSpeed()) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(-17.0f* getSpeed(), playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			}
			airSpeed = dir;

		}
		//Air drag
		if(!canJump) {
			playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX()/1.02f, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
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
		
		//DOUBLE JUMP
		if ((state.buttons.x) && !grounded && canDoubleJump && canPressJump && type == RABBIT) {
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(0,1, 0));
			this->playerObj->getRigidbody()->setLinearVelocity(btVector3(0,0,0));
			this->playerObj->getRigidbody()->applyImpulse(btVector3(0, 205.0f * getJumpHeight() * getWeight() /**deltaTime * 60*/, 0), btVector3(0, 0, 0));
			
			float dir = 17.0f * state.thumbSticks.leftX  * getSpeed();
			playerObj->getRigidbody()->setLinearVelocity(btVector3(dir, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			
			//playerObj->getRigidbody()->applyForce(btVector3(0, 4500.0f, 0), btVector3(0, 0, 0));
			grounded = false;
			canDoubleJump = false;
		}
		if (grounded == true) {
			canDoubleJump = true;
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
			this->playerObj->getRigidbody()->setLinearVelocity(btVector3(0,0, 0));
			this->playerObj->getRigidbody()->applyImpulse(btVector3(0, 205.0f * getJumpHeight() * getWeight() /**deltaTime * 60*/, 0), btVector3(0, 0, 0));
			//this->playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX()*-1, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			//playerObj->getRigidbody()->applyForce(btVector3(0, 4500.0f, 0), btVector3(0, 0, 0));
			grounded = false;
			canWallJump = false;
			wallJumpReset = false;
			System::getSoundManager()->playEffect("4");

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

		
		this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
		//NEW PUNCH
		if (state.buttons.y && canPressPunch) {
			punching = true;
			System::getSoundManager()->playEffect("5");
		}
		//switch (type && punching)
		//{
		//case FOX:
		//	if (grounded) {
		//		playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX() / 1.1, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
		//	}
		//	canPressPunch = false;
		//	hitTimer += 165 * deltaTime;
		//	this->hitbox.hitbox->setPosition(this->getPosition().x + 1.5 * dir, this->getPosition().y, this->getPosition().z);
		//	//this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
		//	if (hitTimer >= 60) {
		//		punching = false;
		//		hitTimer = 0;
		//	}
		//	break;
		//case BEAR:
		//	if (grounded) {
		//		playerObj->getRigidbody()->setLinearVelocity(btVector3(0, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
		//	}
		//	canPressPunch = false;
		//	hitTimer += 125 * deltaTime;
		//	//this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
		//	if (hitTimer > 30 && hitTimer < 50) {
		//		playerObj->getRigidbody()->setLinearVelocity(btVector3(20 * dir, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
		//		this->hitbox.hitbox->setPosition(this->getPosition().x + 1.5 * dir, this->getPosition().y, this->getPosition().z);
		//	}
		//	if (hitTimer >= 60) {
		//		punching = false;
		//		hitTimer = 0;
		//		playerObj->getRigidbody()->setLinearVelocity(btVector3(0, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));

		//	}
		//	break;
		//case RABBIT:
		//	if (grounded) {
		//		playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX() / 1.01, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
		//	}
		//	canPressPunch = false;
		//	hitTimer += 200 * deltaTime;
		//	this->hitbox.hitbox->setPosition(this->getPosition().x + 1.5 * dir, this->getPosition().y, this->getPosition().z);
		//	//this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
		//	if (hitTimer >= 60) {
		//		punching = false;
		//		hitTimer = 0;
		//	}
		//	break;
		//case MOOSE:
		//	if (grounded) {
		//		playerObj->getRigidbody()->applyForce(btVector3(2400.0f * dir, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
		//	}
		//	else {
		//		playerObj->getRigidbody()->applyForce(btVector3(1000.0f * dir, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
		//	}
		//	if (playerObj->getRigidbody()->getLinearVelocity().getX() > 20.0f) {
		//		playerObj->getRigidbody()->setLinearVelocity(btVector3(20.0f, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
		//	}
		//	if (playerObj->getRigidbody()->getLinearVelocity().getX() < -20.0f) {
		//		playerObj->getRigidbody()->setLinearVelocity(btVector3(-20.0f, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
		//	}
		//	canPressPunch = false;
		//	hitTimer += 65 * deltaTime;
		//	this->hitbox.hitbox->setPosition(this->getPosition().x + 1.5 * dir, this->getPosition().y, this->getPosition().z);
		//	//this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
		//	if (hitTimer >= 60) {
		//		punching = false;
		//		hitTimer = 0;
		//	}
		//	break;
		//default:
		//	break;
		//}


		if (punching == true && type == FOX) {
			if (grounded) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX() / 1.1, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
			}
			canPressPunch = false;
			hitTimer += 165 * deltaTime;
			this->hitbox.hitbox->setPosition(this->getPosition().x +1.5*dir, this->getPosition().y, this->getPosition().z);
			//this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
			if (hitTimer >= 60) {
				punching = false;
				hitTimer = 0;
			}
		}
		if (punching == true && type == BEAR) {
			if (grounded) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(0, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
			}
			canPressPunch = false;
			hitTimer += 125 * deltaTime;
			//this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
			if (hitTimer >30 && hitTimer < 50) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(20*dir, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
				this->hitbox.hitbox->setPosition(this->getPosition().x + 1.5*dir, this->getPosition().y, this->getPosition().z);
			}
			if (hitTimer >= 60) {
				punching = false;
				hitTimer = 0;
				playerObj->getRigidbody()->setLinearVelocity(btVector3(0, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
			}
		}
		if (punching == true && type == RABBIT) {
			if (grounded) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX() / 1.01, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
			}
			canPressPunch = false;
			hitTimer += 200 * deltaTime;
			this->hitbox.hitbox->setPosition(this->getPosition().x + 1.5*dir, this->getPosition().y, this->getPosition().z);
			//this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
			if (hitTimer >= 60) {
				punching = false;
				hitTimer = 0;
			}
		}
		if (punching == true && type == MOOSE) {
			if (grounded) {
				playerObj->getRigidbody()->applyForce(btVector3(2400.0f*dir,0.0f,0.0f), btVector3(0.0f, 0.0f, 0.0f));
			}
			else {
				playerObj->getRigidbody()->applyForce(btVector3(1000.0f*dir, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
			}
			if (playerObj->getRigidbody()->getLinearVelocity().getX() > 20.0f) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(20.0f, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			}
			if (playerObj->getRigidbody()->getLinearVelocity().getX() < -20.0f) {
				playerObj->getRigidbody()->setLinearVelocity(btVector3(-20.0f, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
			}
			canPressPunch = false;
			hitTimer += 65 * deltaTime;
			this->hitbox.hitbox->setPosition(this->getPosition().x + 1.5*dir, this->getPosition().y, this->getPosition().z);
			//this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
			if (hitTimer >= 60) {
				punching = false;
				hitTimer = 0;
			}
		}

		if (!state.buttons.y && !punching) {
			canPressPunch = true;
		}


		//OLD PUNCH
		//if (hitbox.time > 0 && hitbox.time < hitbox.totalTime)
		//{
		//	hitbox.time += 1 * deltaTime * 60;
		//}
		//if (hitbox.time >= hitbox.totalTime)
		//{
		//	this->hitbox.hitbox->setPosition(this->getPosition().x, this->getPosition().y, this->getPosition().z);
		//	this->hitbox.time = 0;
		//}
		//float dist = 700;
		//if ((state.buttons.y && canPressPunch) || (this->hitbox.time > 0))
		//{
		//	/*if (this->hitbox.time > 0 &&state.thumbSticks.leftX < 0)
		//	{
		//		dist = -700;
		//	}
		//	else if (this->hitbox.time > 0 && state.thumbSticks.leftX > 0)
		//	{
		//		dist = 700;
		//	}*/
		//	if (grounded) {
		//		playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX() / 1.1, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
		//	}
		//	if (hitbox.time > hitbox.totalTime / 10 && hitbox.time < hitbox.totalTime / 2 && type == BEAR) {
		//		playerObj->getRigidbody()->setLinearVelocity(btVector3(dir*20.0f, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ()));
		//	}
		//	if (hitbox.time > hitbox.totalTime / 2 && type == BEAR) {
		//		playerObj->getRigidbody()->setLinearVelocity(btVector3(playerObj->getRigidbody()->getLinearVelocity().getX() / 1.2, playerObj->getRigidbody()->getLinearVelocity().getY(), playerObj->getRigidbody()->getLinearVelocity().getZ() / 2));
		//	}

		//	if (hitbox.time > hitbox.totalTime / 2)
		//		this->hitbox.hitbox->move((dir*-dist * deltaTime) / hitbox.totalTime, 0, 0);
		//	else
		//	{
		//		this->hitbox.hitbox->move((dir*dist * deltaTime) / hitbox.totalTime, 0, 0);
		//	}
		//	this->hitbox.time++;
		//}

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

		/*if (((state.buttons.y) || (state.buttons.a)) && canPressPunch) {
			System::getSoundManager()->playEffect("5");
		}

		if ((state.buttons.y) || (state.buttons.a)) {
				canPressPunch = false;
		}
		else {
				canPressPunch = true;
		}*/

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
		if (!(punching == true && type == MOOSE)) {
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
		}
		if (facing > (3.14f / 2.f)) {
			facing = 3.14f/ 2.f;
		}
		if (facing < (-3.14f /2.f)) {
			facing = -3.14f / 2.f;
		}
		this->playerObj->setRotationRollPitchYaw(this->playerObj->getRotation().x, facing, this->playerObj->getRotation().z);
	}

	grounded = false;

	if (isDead()) {
		//this->playerObj->getRigidbody()->setLinearFactor(btVector3(1, 1, 0));
		//this->playerObj->getRigidbody()->setAngularFactor(btVector3(0, 0, 1));

		//playerObj->setRotationRollPitchYaw()
		XMVECTOR temp;
		temp.m128_f32[0] = this->playerObj->getRigidbody()->getWorldTransform().getRotation().getX();
		temp.m128_f32[1] = this->playerObj->getRigidbody()->getWorldTransform().getRotation().getY();
		temp.m128_f32[2] = this->playerObj->getRigidbody()->getWorldTransform().getRotation().getZ();
		temp.m128_f32[3] = this->playerObj->getRigidbody()->getWorldTransform().getRotation().getW();

		//playerObj->setRotationRollPitchYaw(getRoll(temp) * -1, playerObj->getRotation().y, playerObj->getRotation().z);


		//this->playerObj->setRotation(this->playerObj->getRigidbody()->getWorldTransform().getRotation().getX(), this->playerObj->getRigidbody()->getWorldTransform().getRotation().getY()*3.14 * 2, this->playerObj->getRigidbody()->getWorldTransform().getRotation().getZ()*3.14 * 2, this->playerObj->getRigidbody()->getWorldTransform().getRotation().getW()*3.14 * 2);
		//this->playerObj->setRotationRollPitchYaw(this->playerObj->getRigidbody()->getWorldTransform().getRotation().getX() *3.14 * 2,
		//this->playerObj->getRigidbody()->getWorldTransform().getRotation().getY() *3.14 * 2,
		//this->playerObj->getRigidbody()->getWorldTransform().getRotation().getZ() *3.14 * 2);
		//this->playerObj->getRigidbody()->getWorldTransform().getRotation().getW() *3.14 * 2);

			//this->playerObj->setRotation(0, 0,1, this->playerObj->getRigidbody()->getWorldTransform().getRotation().getAngle());
			//this->playerObj->setRotation(this->playerObj->getRigidbody()->getWorldTransform().getRotation().getX(), this->playerObj->getRotation().y, this->playerObj->getRotation().z, this->playerObj->getRigidbody()->getWorldTransform().getRotation().getAngle());
		deathTimer += 60 * deltaTime;
		if (deathTimer >= 100) {
			health = getMaxHealth();
			deathTimer = 0;
			setRigidbodyPosition(0,+20,0);
			playerObj->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));
		}
	}
	//Fixar bug så att man inte kan hoppa på plattformar
	groundTimer += deltaTime * 1000;
	if (groundTimer >= 100) {
		grounded = false;
		groundTimer = 100;
	}
	//grounded = false;

	//playerObj->computeAnimationMatrix(deltaTime*animSpeed);
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

int Player::getNextAnimal()
{
	return this->nextAnimal;
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
float Player::getPitch(DirectX::XMVECTOR Quaternion)
{
	return atan2(2 * (Quaternion.m128_f32[1] * Quaternion.m128_f32[2] + Quaternion.m128_f32[3] * Quaternion.m128_f32[0]), Quaternion.m128_f32[3] * Quaternion.m128_f32[3] - Quaternion.m128_f32[0] * Quaternion.m128_f32[0] - Quaternion.m128_f32[1] * Quaternion.m128_f32[1] + Quaternion.m128_f32[2] * Quaternion.m128_f32[2]);
}

float Player::getYaw(DirectX::XMVECTOR Quaternion)
{
	return asin(-2 * (Quaternion.m128_f32[0] * Quaternion.m128_f32[2] - Quaternion.m128_f32[3] * Quaternion.m128_f32[1]));
}

float Player::getRoll(DirectX::XMVECTOR Quaternion)
{
	return atan2(2 * (Quaternion.m128_f32[0] * Quaternion.m128_f32[1] + Quaternion.m128_f32[3] * Quaternion.m128_f32[2]), Quaternion.m128_f32[3] * Quaternion.m128_f32[3] + Quaternion.m128_f32[0] * Quaternion.m128_f32[0] - Quaternion.m128_f32[1] * Quaternion.m128_f32[1] - Quaternion.m128_f32[2] * Quaternion.m128_f32[2]);
}