#include "Animal.h"
#include "System.h"

// AnimalDef { maxHealh, weight, speed, modelPath };
AnimalDef Animal::fox = { 100, 10.0F, 1.0F, "Resources/Models/fox_character_run.lu" };
AnimalDef Animal::bear = { 120, 50.0F, 0.6F, "NOT_MADE" };

const AnimalDef& Animal::getAnimal(AnimalType type)
{
	switch (type)
	{
	case FOX:
		return Animal::fox;
	case BEAR:
		return Animal::bear;
	}

	return Animal::fox;
}
