#include "Animal.h"
#include "System.h"

// AnimalDef			   { maxHealh,	weight,	speed,	jumpHeight,	strength,	modelPath };
AnimalDef Animal::fox =    { 100,		1.0F,	1.2F,	1.0F,		5,			"Resources/Models/fox_character_run.lu" };
AnimalDef Animal::bear =   { 200,		2.0F,	0.6F,	0.8F,		15,			"Resources/Models/cube2.lu" };
AnimalDef Animal::rabbit = { 75,		0.5F,	1.0F,	1.4F,		5,			"Resources/Models/tree1.lu" };
AnimalDef Animal::moose =  { 175,		2.0F,	0.6F,	0.8F,		10,			"Resources/Models/platform1.lu" };


const AnimalDef& Animal::getAnimal(AnimalType type)
{
	switch (type)
	{
	case FOX:
		return Animal::fox;
	case BEAR:
		return Animal::bear;
	case RABBIT:
		return Animal::rabbit;
	case MOOSE:
		return Animal::moose;
	}

	return Animal::fox;
}
