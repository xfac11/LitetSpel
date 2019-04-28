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
			this->transModels[this->nrOfTrans].worldPtr = &gameObject->getWorld();//&btTransform_to_XMMATRIX(gameObject->body()->getWorldTransform());
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
	//float pos[4] = {
	//3.0,0.0,0.0,10.0f
	//};
	//DirectX::XMMATRIX worldPos = DirectX::XMMatrixTranslation(pos[0], pos[1], pos[2]);
	//this->lightsCB.data.lights[1].worldLight=worldPos;//sun

	//this->lightsCB.data.lights[0].position[0] = pos[0];//sun
	//this->lightsCB.data.lights[0].position[1] = pos[1];
	//this->lightsCB.data.lights[0].position[2] = pos[2];
	//this->lightsCB.data.lights[0].position[3] = pos[3];
	//this->lightsCB.data.lights[0].color[0] = 1.0f;
	//this->lightsCB.data.lights[0].color[1] = 0.0f;
	//this->lightsCB.data.lights[0].color[2] = 0.0f;
	//this->lightsCB.data.lights[0].color[3] = 0.0f;
	//this->lightsCB.data.lights[0].direction[0] = 0.0f;
	//this->lightsCB.data.lights[0].direction[1] = 1.0f;
	//this->lightsCB.data.lights[0].direction[2] = 0.0f;
	//this->lightsCB.data.lights[0].direction[3] = 1.0f;

	//this->lightsCB.data.lights[1].position[0] = pos[0];//pointlights
	//this->lightsCB.data.lights[1].position[1] = pos[1];
	//this->lightsCB.data.lights[1].position[2] = pos[2];
	//this->lightsCB.data.lights[1].position[3] = pos[3];
	//this->lightsCB.data.lights[1].color[0] = 0.0f;
	//this->lightsCB.data.lights[1].color[1] = 1.0f;
	//this->lightsCB.data.lights[1].color[2] = 0.0f;
	//this->lightsCB.data.lights[1].color[3] = 1.0f;
	//this->lightsCB.data.nrOfLights = 2;
	for (int i = 0; i < this->nrOfOpaque; i++)
	{
		this->opaqueModels[i].modelPtr->getShader()->setWorld(*this->opaqueModels[i].worldPtr);
		DirectX::XMMATRIX worldPos = DirectX::XMMatrixTranslation(gameObjects[2]->getPosition().x, gameObjects[2]->getPosition().y, gameObjects[2]->getPosition().z);
		this->lightsCB.data.lights[1].worldLight = worldPos;
		this->lightsCB.data.nrOfLights = nrOfLights;
		this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
		this->opaqueModels[i].modelPtr->getShader()->setConstanbuffer(PIXEL, 1, this->lightsCB.getBuffer());
		this->opaqueModels[i].modelPtr->draw();
	}
	for (int i = 0; i < this->nrOfTrans; i++)
	{
		this->transModels[i].modelPtr->getShader()->setWorld(*this->transModels[i].worldPtr);
		DirectX::XMMATRIX worldPos = DirectX::XMMatrixTranslation(gameObjects[2]->getPosition().x, gameObjects[2]->getPosition().y, gameObjects[2]->getPosition().z);
		this->lightsCB.data.lights[1].worldLight = worldPos;
		this->lightsCB.data.nrOfLights = nrOfLights;
		this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
		this->transModels[i].modelPtr->getShader()->setConstanbuffer(PIXEL, 1, this->lightsCB.getBuffer());
		this->transModels[i].modelPtr->draw();
	}
}

void GameObjectHandler::initialize()
{
	this->lightsCB.initialize(System::getDevice());
}

void GameObjectHandler::addLight(float pos[4],float dir[4],float color[4] )
{
	if (nrOfLights != 16)
	{
		DirectX::XMMATRIX worldPos = DirectX::XMMatrixTranslation(pos[0], pos[1], pos[2]);
		this->lightsCB.data.lights[nrOfLights].worldLight = worldPos;//sun
		for (int i = 0; i < 4; i++)
		{
			this->lightsCB.data.lights[nrOfLights].position[i] = pos[i];
			this->lightsCB.data.lights[nrOfLights].direction[i] = dir[i];
			this->lightsCB.data.lights[nrOfLights].color[i] = color[i];
		}
		nrOfLights++;
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
