#pragma once
#include "Geomety.h"
class Animal
{
protected:
	float health;
	float weight;
	float speed;
	AABB body;
public:
	Animal();
	~Animal();
	float GetHealth()const;
	void SetDmg(float d);

	AABB GetBody();
};

