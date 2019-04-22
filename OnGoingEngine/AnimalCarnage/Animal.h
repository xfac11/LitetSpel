#pragma once
#include "Geometry.h"
class Animal : public GameObject
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

