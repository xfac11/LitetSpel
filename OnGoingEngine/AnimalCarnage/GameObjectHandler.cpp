#include"GameObjectHandler.h"

GameObjectHandler::GameObjectHandler()
{
	this->nrOfObjects = 0;
	this->cap = 5;
	this->gameObjects = new GameObject*[this->cap];
	for (int i = 0; i < this->cap; i++)
	{
		this->gameObjects[i] = nullptr;
	}
}

GameObjectHandler::~GameObjectHandler()
{
	for (int i = 0; i < this->nrOfObjects; i++)
	{
		delete this->gameObjects[i];
	}
	delete[] this->gameObjects;
}

void GameObjectHandler::addObject(ShaderType type, GameObject *& gameObject)
{
	if (this->nrOfObjects == this->cap)
	{
		this->expand();
	}
	this->gameObjects[this->nrOfObjects] = gameObject;
	this->nrOfObjects++;
}

void GameObjectHandler::addObject()
{
}

void GameObjectHandler::expand()
{
}