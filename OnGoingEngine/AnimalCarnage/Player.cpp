#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

void Player::SelectCharacter(CHARACTER b)
{
	
	switch (b)
	{
	case BEAR:
		this->animal[0] = Bear();
		this->current = 0;
		break;
	case FOX:
		this->animal[1] = Bear();
		this->current = 1;
		break;
	}
}

void Player::update()
{
}

AABB Player::GetBody()
{
	return 	this->animal[current].GetBody();
}
