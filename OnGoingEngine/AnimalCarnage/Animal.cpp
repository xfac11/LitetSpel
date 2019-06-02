#include "Animal.h"
#include "System.h"


std::vector<std::string> rabbitV = { "_idle", "_run","_jump_falling","_jump_landing","_jump_start","_attack","_death" };
std::vector<std::string> foxV = { "_idle", "_run","_jump_falling","_jump_landing","_jump_start","_attack","_death" };
std::vector<std::string> bearV = {"_idle"};
std::vector<std::string> mooseV = {};


// AnimalDef			   { maxHealh,	weight,	speed,	jumpHeight,	strength,	modelPath };
AnimalDef Animal::fox =	   { 100,		1.0F,	1.2F,	1.0F,		10,			"Resources/Models/fox_character",		"fox_character_mask.tga",		foxV,		"R_front_paw" };
AnimalDef Animal::bear =   { 200,		2.0F,	0.6F,	0.8F,		15,			"Resources/Models/bear_character",		"fox_character_mask.tga",		bearV,		"R_front_paw" };
AnimalDef Animal::rabbit = { 75,		0.5F,	1.0F,	1.0F,		10,			"Resources/Models/rabbit_character",	"rabbit_character_mask.tga",	rabbitV,	"head" };
AnimalDef Animal::moose =  { 175,		2.0F,	0.6F,	0.8F,		12,			"Resources/Models/rabbit_character",	"fox_character_mask.tga",		rabbitV ,	"head" };


AnimalDef& Animal::getAnimal(AnimalType type)
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
