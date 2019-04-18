#include "Animal.h"



Animal::Animal()
{

}

Animal::~Animal()
{
}

float Animal::GetHealth() const
{
	return this->health;
}

void Animal::SetDmg(float d)
{
	this->health -= d;
}

AABB Animal::GetBody()
{
	//body.position = this->getPosition();
	////body.width = getWidth();
	////body.height = getHeight();
	 AABB body;
	 return body;
}
