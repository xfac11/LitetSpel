#ifndef ANIMAL_H
#define ANIMAL_H

#include "GameObject.h"

enum AnimalType
{
	DEFAULT_TYPE,
	FOX,
	BEAR
};

struct AnimalDef
{
	int maxHealh;
	float weight;
	float speed;
	float jumpHeight;
	int strength;
	const char* modelPath;
};

class Animal
{
private:
	static AnimalDef fox;
	static AnimalDef bear;

public:
	static const AnimalDef& getAnimal(AnimalType type);
};

#endif // !ANIMAL_H
