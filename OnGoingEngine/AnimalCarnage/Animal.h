#ifndef ANIMAL_H
#define ANIMAL_H

#include "GameObject.h"

enum AnimalType
{
	FOX,
	BEAR,
	RABBIT,
	MOOSE
};

struct AnimalDef
{
	int maxHealh;
	float weight;
	float speed;
	float jumpHeight;
	int strength;
	const char* modelPath;
	const char* maskPath;
	std::vector<std::string> animalAnimations;
};

class Animal
{
private:
	static AnimalDef fox;
	static AnimalDef bear;
	static AnimalDef rabbit;
	static AnimalDef moose;

public:
	static const AnimalDef& getAnimal(AnimalType type);
};

#endif // !ANIMAL_H
