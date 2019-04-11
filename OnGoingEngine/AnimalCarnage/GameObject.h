#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include"Model.h"
class GameObject
{
public:
	GameObject();
	~GameObject();


private:
	Model theModel;

	//Hitbox theHitbox; a model with just the index and vertices and a color. no textures etc
	//Transform theTransforms;
};
#endif // !GAMEOBJECT_H
