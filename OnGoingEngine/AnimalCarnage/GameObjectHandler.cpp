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
			gameObject->getModel(i)->setShader(System::shaderManager->getDefShader());
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

	this->lightsCB.data.nrOfLights = nrOfLights;
	DirectX::XMMATRIX worldPos = DirectX::XMMatrixTranslation(gameObjects[2]->getPosition().x, gameObjects[2]->getPosition().y, gameObjects[2]->getPosition().z);
	this->lightsCB.data.lights[1].worldLight = worldPos;
	this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
	float color[] = {
		0,0,0,1.0f
	};
	System::theGraphicDevice->turnOnZ();
	System::shaderManager->getDefShader()->setCBuffers();
	System::shaderManager->getDefShader()->setConstanbuffer(PIXEL, 1, this->lightsCB.getBuffer());
	System::shaderManager->getDefShader()->setShaders();
	System::shaderManager->getDefShader()->prepGBuffer(color);
	for (int i = 0; i < this->nrOfOpaque; i++)
	{
		this->opaqueModels[i].modelPtr->getShader()->setWorld(*this->opaqueModels[i].worldPtr);
		this->opaqueModels[i].modelPtr->draw();
	}
	

	System::theGraphicDevice->setBackBuffer();
	System::theGraphicDevice->beginScene(color);
	System::shaderManager->getDefShader()->prepForLight();
	System::theGraphicDevice->setBackBuffer(System::shaderManager->getDefShader()->gBuffer.getDepthStcView());
	//
	System::shaderManager->getLightShader()->setCBuffers();
	System::shaderManager->getLightShader()->setConstanbuffer(PIXEL, 1, this->lightsCB.getBuffer());
	//System::theGraphicDevice->turnOffZ();

	UINT32 offset = 0; 
	this->lightsCB.data.index = 0;
	this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
	System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBufferQuad.GetAddressOf(), &*vertexBufferQuad.getStridePtr(), &offset);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	System::shaderManager->getLightShader()->renderShaderDir(quad.size(), System::shaderManager->getDefShader()->gBuffer.getDepthStcView());
	for (int i = 1; i < nrOfLights; i++)
	{
		this->lightsCB.data.index = i;
		this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
		System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBufferQuad.GetAddressOf(), &*vertexBufferQuad.getStridePtr(), &offset);
		System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		System::shaderManager->getLightShader()->renderShaderDir(quad.size(), System::shaderManager->getDefShader()->gBuffer.getDepthStcView());
	}
	System::theGraphicDevice->turnOnZ();
	System::theGraphicDevice->setRasterState();
	System::theGraphicDevice->setBlendState();
	//Light render sphere etc


	////Forward
	System::shaderManager->getForwardShader()->setShaders();//tänker att man kör denna sen renderar allla som använder denna shader sen tar setshader på nästa osv.
	System::shaderManager->getForwardShader()->setCBuffers();
	System::shaderManager->getForwardShader()->setConstanbuffer(PIXEL, 1, this->lightsCB.getBuffer());

	for (int i = 0; i < this->nrOfTrans; i++)
	{
		this->transModels[i].modelPtr->getShader()->setWorld(*this->transModels[i].worldPtr);
		this->transModels[i].modelPtr->draw();
	}
	
}

void GameObjectHandler::initialize()
{
	this->lightsCB.initialize(System::getDevice());
	LightVertex temp;
	temp.position[0] = 1.0f;
	temp.position[1] = 1.0f;
	temp.position[2] = 0.0f;
	temp.uv[0] = 0.0f;
	temp.uv[1] = 0.0f;
	quad.push_back(temp);

	temp.position[0] = 1.0f;
	temp.position[1] = 1.0f;
	temp.position[2] = 0.0f;
	temp.uv[0] = 1.0f;
	temp.uv[1] = 1.0f;
	quad.push_back(temp);

	temp.position[0] = 1.0f;
	temp.position[1] = 1.0f;
	temp.position[2] = 0.0f;
	temp.uv[0] = 0.0f;
	temp.uv[1] = 1.0f;
	quad.push_back(temp);

	temp.position[0] = 1.0f;
	temp.position[1] = 1.0f;
	temp.position[2] = 0.0f;
	temp.uv[0] = 0.0f;
	temp.uv[1] = 0.0f;
	quad.push_back(temp);

	temp.position[0] = 1.0f;
	temp.position[1] = 1.0f;
	temp.position[2] = 0.0f;
	temp.uv[0] = 1.0f;
	temp.uv[1] = 0.0f;
	quad.push_back(temp);

	temp.position[0] = 1.0f;
	temp.position[1] = 1.0f;
	temp.position[2] = 0.0f;
	temp.uv[0] = 1.0f;
	temp.uv[1] = 1.0f;
	quad.push_back(temp);
	
	this->vertexBufferQuad.initialize(quad.data(), quad.size(), System::getDevice());

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
		this->lightsCB.data.index = nrOfLights;
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

void GameObjectHandler::renderToTexture(float* color)
{
	
	System::shaderManager->getDefShader()->prepGBuffer(color);

}

void GameObjectHandler::deferredRender()
{
}
