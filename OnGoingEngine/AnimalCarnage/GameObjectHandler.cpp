#include"GameObjectHandler.h"
#include"System.h"
GameObjectHandler::GameObjectHandler()
{
	this->nrOfObjects = 0;
	this->nrOfOpaque = 0;
	this->nrOfTrans = 0;
	this->cap = 5;
	this->capOpaque = 5;
	this->capTrans = 5;
	this->opaqueModels = new ModWorld[this->capOpaque];
	this->transModels = new ModWorld[this->capTrans];
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
	delete[] this->transModels;
	delete[] this->opaqueModels;
}

void GameObjectHandler::addObject(GameObject *& gameObject)
{
	if (this->nrOfObjects == this->cap)
	{
		this->expandGameobjects();
	}
	this->gameObjects[this->nrOfObjects] = gameObject;
	this->nrOfObjects++;
	
	for (int i = 0; i < gameObject->getNrOfModels(); i++)
	{
		if (gameObject->getModel(i)->getOpacity() == Transparent)
		{
			if (this->nrOfTrans == this->capTrans)
				this->expandTransModels();
			//gameObject->getModel(i).setShader(Shader);
			this->transModels[this->nrOfTrans].modelPtr = gameObject->getModel(i);
			this->transModels[this->nrOfTrans].worldPtr = &gameObject->getWorld();
			gameObject->getModel(i)->setShader(System::shaderManager->getForwardShader());
			this->nrOfTrans++;
			/*Model* *ptr = new Model*[4];
			ptr[0] = new Model;
			ptr[0]->setTexture("box.tga");
			gameObject->getTheModelPtr() = ptr;*/ 
			//gameObject->getTheModelPtr() = new Model*[54];
		}
		else
		{
			if (this->nrOfOpaque == this->capOpaque)
				this->expandOpaqueModels();
			this->opaqueModels[this->nrOfOpaque].modelPtr = gameObject->getModel(i);
			this->opaqueModels[this->nrOfOpaque].worldPtr = &gameObject->getWorld();
			gameObject->getModel(i)->setShader(System::shaderManager->getForwardShader());
			this->nrOfOpaque++;
		}
	}
}

void GameObjectHandler::addObject(char* file)
{
	
	//Modelloader
	//readfile(file);
}

GameObject & GameObjectHandler::getObject(int id)
{
	return *this->gameObjects[id];
}

void GameObjectHandler::draw()
{
	for (int i = 0; i < this->nrOfOpaque; i++)
	{
		this->opaqueModels[i].modelPtr->getShader()->setWorld(*this->opaqueModels[i].worldPtr);
		this->opaqueModels[i].modelPtr->draw();
	}
}

void GameObjectHandler::expandGameobjects()
{
	this->cap += 5;
	GameObject* *temp = new GameObject*[this->cap];
	for (int i = 0; i < this->nrOfObjects; i++)
	{
		temp[i] = this->gameObjects[i];
	}
	delete[]this->gameObjects;
	this->gameObjects = temp;
}

void GameObjectHandler::expandTransModels()
{
	this->capTrans += 5;
	ModWorld* temp = new ModWorld[this->capTrans];
	for (int i = 0; i < this->nrOfTrans; i++)
	{
		temp[i] = this->transModels[i];
	}
	delete[]this->transModels;
	this->transModels = temp;
}

void GameObjectHandler::expandOpaqueModels()
{
	this->capOpaque += 5;
	ModWorld* temp = new ModWorld[this->capOpaque];
	for (int i = 0; i < this->nrOfOpaque; i++)
	{
		temp[i] = this->opaqueModels[i];
	}
	delete[]this->opaqueModels;
	this->opaqueModels = temp;
}
