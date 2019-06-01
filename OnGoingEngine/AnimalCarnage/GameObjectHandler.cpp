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
	this->lightSphereWorld = new DirectX::XMMATRIX[16];//16 cap of light
	for (int i = 0; i < this->cap; i++)
	{
		this->gameObjects[i] = nullptr;
	}

	//this->animTimer = 0;
	//this->animTimer = 0;
}

GameObjectHandler::~GameObjectHandler()
{
	for (int i = 0; i < this->nrOfObjects; i++)
	{
		delete this->gameObjects[i];
	}
	delete[] this->lightSphereWorld;
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
		if (gameObject->getModel()->getOpacity() == Transparent)
		{
			if (this->nrOfTrans == this->capTrans)
				this->expandTransModels();
			//gameObject->getModel(i).setShader(Shader);
			this->transModels[this->nrOfTrans].modelPtr = gameObject->getModel();
			this->transModels[this->nrOfTrans].worldPtr = &gameObject->getWorld();//&btTransform_to_XMMATRIX(gameObject->body()->getWorldTransform());
			this->transModels[this->nrOfTrans].selfPtr = gameObject;
			gameObject->getModel()->setShader(System::shaderManager->getForwardShader());
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
			this->opaqueModels[this->nrOfOpaque].modelPtr = gameObject->getModel();
			this->opaqueModels[this->nrOfOpaque].worldPtr = &gameObject->getWorld();
			this->opaqueModels[this->nrOfOpaque].selfPtr = gameObject;
			gameObject->getModel()->setShader(System::shaderManager->getDefShader());
			this->nrOfOpaque++;
		}
	}
}

//void GameObjectHandler::addObject(char* file)
//{
//	
//	//Modelloader
//	//readfile(file);
//}

GameObject & GameObjectHandler::getObject(int id)
{
	return *this->gameObjects[id];
}

void GameObjectHandler::draw(float deltaTime, bool isPaused, std::vector<float> & playerSpeed, std::vector<string> & playerName, std::vector<bool> & playerLoop)
{
	//Shadow
	System::theGraphicDevice->setRasterState();
	lightView = DirectX::XMMatrixLookAtLH(lightCamPos, lightDirView, up);//Måste kalkuleras varje frame av någon anledning
	System::shaderManager->getShadowMapping()->prepare(lightView);//setshader + omsetrendertarget(0,0,depthstencilview

	for (int i = 0; i < this->nrOfOpaque; i++)
	{
		if (i != 3&&this->opaqueModels[i].selfPtr->getActiveDraw())
		{
			System::shaderManager->getShadowMapping()->setWorld(*this->opaqueModels[i].worldPtr);
			this->opaqueModels[i].selfPtr->getModel()->drawOnlyVertex();
		}
	}
	//Reset and prep for deferred rendering
	System::theGraphicDevice->setViewPort();
	//System::theGraphicDevice->setRasterState();// om shadow mapping körs med front så
	System::getDeviceContext()->OMSetBlendState(nullptr, blendFactor, 0xffffffff);
	
	System::shaderManager->getDefShader()->prepDeferredRendering(color);
	//sets the constantbuffer set shaders set rendertargets to gbuffer and clear with color

	System::theGraphicDevice->setRasterBack();
	int index = 0;
	for (int i = 0; i < this->nrOfOpaque; i++)
	{
		if (this->opaqueModels[i].selfPtr->getActiveDraw())
		{
			//has skeleton? then calculate matrix  anim
			GameObject* gameObjectPtr = this->opaqueModels[i].selfPtr;
			shared_ptr<Model> ptr = gameObjectPtr->getModel();
			if (this->opaqueModels[i].selfPtr->haveAnimation() == true && index < playerSpeed.size()) //isPaused == false && 
			{
				//this->timePassed += deltaTime;
				

				this->opaqueModels[i].selfPtr->computeAnimationMatrix(deltaTime*playerSpeed[index]*!isPaused, playerName[index], playerLoop[index]);//playerName[index] //run_cycle, idle

			    index++;
			}

			System::shaderManager->getDefShader()->setRepeat(gameObjectPtr->getRepeat());
			System::shaderManager->getDefShader()->setMaskColor(gameObjectPtr->getColorMask());
			System::shaderManager->getDefShader()->setWorld(*this->opaqueModels[i].worldPtr);
			ptr->draw();

			
		}
	}

	System::shaderManager->getDefShader()->resetRenderTargets();

	System::theGraphicDevice->turnOffZ();
	//glow map blur
	UINT32 offset = 0;
	System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBufferQuad.GetAddressOf(), &*vertexBufferQuad.getStridePtr(), &offset);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	System::shaderManager->getHorBlur()->prepare();
	System::shaderManager->getHorBlur()->render(quad.size(), System::shaderManager->getDefShader()->gBuffer.getShadResView(3));
	System::shaderManager->getVerBlur()->prepare();
	System::shaderManager->getVerBlur()->render(quad.size(), System::shaderManager->getHorBlur()->getShaderView());
	//light
	System::getDeviceContext()->PSSetShaderResources(3, 1, &System::shaderManager->getShadowMapping()->getShadowMap());
	System::shaderManager->getLightShader()->setShaders();
	System::shaderManager->getDefShader()->prepForLight();
	System::theGraphicDevice->setBackBuffer(System::shaderManager->getDefShader()->gBuffer.getDepthStcView());
	//this->lightsCB.data.index = 0;
	//this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());

	System::getDeviceContext()->PSSetShaderResources(4, 1, &System::shaderManager->getVerBlur()->getShaderView());
	System::shaderManager->getLightShader()->setCBuffers();
	System::shaderManager->getLightShader()->setConstanbuffer(PIXEL, 1, this->lightsCB.getBuffer());
	System::shaderManager->getLightShader()->setTypeOfLight(0);//0 directlight
	this->lightsCB.data.index = 0;
	this->lightsCB.data.nrOfLights = nrOfLights;
	this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
	System::shaderManager->getShadowMapping()->setPSDepthView();//sets the mvp for the depth in lspixelshader

	System::shaderManager->getLightShader()->renderShaderDir((int)quad.size());//render fullscreen quad with all lights
	UINT32 offsetS = 0;
	System::getDeviceContext()->IASetIndexBuffer(indexBufferSphere.getBuffer(), DXGI_FORMAT_R32_UINT, 0);
	System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBufferSphere.GetAddressOf(), &*vertexBufferSphere.getStridePtr(), &offsetS);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	System::shaderManager->getLightShader()->setTypeOfLight(1);//1 pointlight

	for (int i = 1; i < nrOfLights; i++)
	{
		//DirectX::XMMATRIX worldSphere = DirectX::XMMatrixTranspose(XMMatrixScaling(this->lightsCB.data.lights[i].position[3] * 10, this->lightsCB.data.lights[i].position[3] * 10, this->lightsCB.data.lights[i].position[3] * 10)*
			//this->lightsCB.data.lights[i].worldLight);
		this->lightsCB.data.index = i;
		this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
		System::shaderManager->getLightShader()->setTheWorld(this->lightSphereWorld[i]);
		System::shaderManager->getLightShader()->renderShaderPoint((int)sphereIndices.size(), System::shaderManager->getDefShader()->gBuffer.getDepthStcView());
	}

	System::theGraphicDevice->turnOnZ();
	System::theGraphicDevice->setRasterState();
	System::getDeviceContext()->PSSetShaderResources(3, 0, nullptr);

	//Light render sphere etc
	//Skybox
	System::theGraphicDevice->setBlendState();
	System::skybox->setCB();
	System::skybox->render();

	
	/*for (int i = 0; i < this->nrOfOpaque; i++)
	{
		this->opaqueModels[i].modelPtr->getShader()->setWorld(*this->opaqueModels[i].worldPtr);
		this->opaqueModels[i].modelPtr->draw();
	}*/

	////Forward
	//back to front sorting here
	//this->sortBackToFront();//do it every 30 frames
	System::shaderManager->getParticleShader()->setCBuffers();
	System::shaderManager->getParticleShader()->setShaders();
	System::getParticleManager()->render();
	System::shaderManager->getForwardShader()->setShaders();//tänker att man kör denna sen renderar allla som använder denna shader sen tar setshader på nästa osv.
	System::shaderManager->getForwardShader()->setCBuffers();
	System::shaderManager->getForwardShader()->setConstanbuffer(PIXEL, 1, this->lightsCB.getBuffer());
	System::shaderManager->getShadowMapping()->setCBViewAndProj();//sets the vertex constantbuffer (if no gs need to change this to two view and proj)
	System::shaderManager->getShadowMapping()->setSampler();
	
	System::getDeviceContext()->PSSetShaderResources(4, 1, &System::shaderManager->getShadowMapping()->getShadowMap());
	if (this->nrOfTrans > 0)
	{
		for (int i = 0; i < this->nrOfTrans; i++)
		{
			this->transModels[i].modelPtr->getShader()->setWorld(*this->transModels[i].worldPtr);
			this->transModels[i].modelPtr->draw();
		}
	}
	


	/*for (int i = 0; i < this->nrOfOpaque; i++)
	{
		this->opaqueModels[i].modelPtr->getShader()->setWorld(*this->opaqueModels[i].worldPtr);
		this->opaqueModels[i].modelPtr->draw();
	}*/

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

	this->vertexBufferQuad.initialize(quad.data(), (UINT)quad.size(), System::getDevice());
	this->generateSphere();
}

void GameObjectHandler::setSunDir(DirectX::XMFLOAT3 dir)
{
	this->lightsCB.data.lights[0].direction[0] = dir.x;
	this->lightsCB.data.lights[0].direction[1] = dir.y;
	this->lightsCB.data.lights[0].direction[2] = dir.z;
	this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());

}
void GameObjectHandler::setLightPos(int id, btVector3 pos)
{
	
	DirectX::XMMATRIX worldPos = DirectX::XMMatrixTranslation(pos[0], pos[1], pos[2]);
	
	for (int i = 0; i < 3; i++)
	{
		this->lightsCB.data.lights[id].position[i] = pos[i];
	}
	this->lightsCB.data.lights[id].worldLight = worldPos;
	
	lightSphereWorld[id] = DirectX::XMMatrixTranspose(XMMatrixScaling(this->lightsCB.data.lights[id].position[3] * 10, this->lightsCB.data.lights[id].position[3] * 10, this->lightsCB.data.lights[id].position[3] * 10)*
		this->lightsCB.data.lights[id].worldLight);

	this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
}
void GameObjectHandler::reset()
{

	for (int i = 0; i < this->nrOfObjects; i++)
	{
		delete this->gameObjects[i];
	}
	delete[] this->lightSphereWorld;
	delete[] this->gameObjects;
	delete[] this->transModels;
	delete[] this->opaqueModels;

	for (int i = 0; i < this->nrOfLights; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->lightsCB.data.lights[i].direction[j] = 0.0f;
			this->lightsCB.data.lights[i].position[j] = 0.0f;
			this->lightsCB.data.lights[i].cameraPos[j] = 0.0f;
			this->lightsCB.data.lights[i].color[j] = 0.0f;
		}
		this->lightsCB.data.lights[i].worldLight = DirectX::XMMatrixIdentity();
	}
	this->lightsCB.data.nrOfLights = 0;
	this->lightsCB.data.index = 0;
	this->lightsCB.applyChanges(System::getDevice(),System::getDeviceContext());
	this->nrOfLights = 0;
	this->nrOfObjects = 0;
	this->nrOfOpaque = 0;
	this->nrOfTrans = 0;
	this->cap = 5;
	this->capOpaque = 5;
	this->capTrans = 5;
	this->opaqueModels = new ModWorld[this->capOpaque];
	this->transModels = new ModWorld[this->capTrans];
	this->gameObjects = new GameObject*[this->cap];
	this->lightSphereWorld = new DirectX::XMMATRIX[16];//16 cap of light
	for (int i = 0; i < this->cap; i++)
	{
		this->gameObjects[i] = nullptr;
	}

}
void GameObjectHandler::addLight(float pos[4], float dir[4], float color[4])
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
		if (nrOfLights == 0)
		{
			
			float lightViewLengt = 7;
			lightCamPos = DirectX::XMVectorSet(lightViewLengt * (-1 * this->lightsCB.data.lights[0].direction[0]), lightViewLengt * (-1 * this->lightsCB.data.lights[0].direction[1]), lightViewLengt * (this->lightsCB.data.lights[0].direction[2]), 1);
			
		}
		lightSphereWorld[nrOfLights] = DirectX::XMMatrixTranspose(XMMatrixScaling(this->lightsCB.data.lights[nrOfLights].position[3] * 10, this->lightsCB.data.lights[nrOfLights].position[3] * 10, this->lightsCB.data.lights[nrOfLights].position[3] * 10)*
			this->lightsCB.data.lights[nrOfLights].worldLight);
		nrOfLights++;
	}
}

void GameObjectHandler::setSkyboxTexture(std::string file)
{
	System::skybox->setTexture(file);
}

void GameObjectHandler::generateSphere()
{
	//double latitudeBands = 5;
	//double longitudeBands = 5;
	//double radius = 0.5f;


	int stacks = 5;
	int slices = 5;
	const float PI = 3.14f;

	LightVertex vertex;
	// loop through stacks.
	for (int i = 0; i <= stacks; ++i) {

		float V = (float)i / (float)stacks;
		float phi = V * PI;

		// loop through the slices.
		for (int j = 0; j <= slices; ++j) {

			float U = (float)j / (float)slices;
			float theta = U * (PI * 2);

			// use spherical coordinates to calculate the positions.
			vertex.position[0] = cos(theta) * sin(phi);
			vertex.position[1] = cos(phi);
			vertex.position[2] = sin(theta) * sin(phi);

			sphereVector.push_back(vertex);
		}
	}

	// Calc The Index Positions
	for (int i = 0; i < slices * stacks + slices; ++i) {
		sphereIndices.push_back(DWORD(i));
		sphereIndices.push_back(DWORD(i + slices + 1));
		sphereIndices.push_back(DWORD(i + slices));

		sphereIndices.push_back(DWORD(i + slices + 1));
		sphereIndices.push_back(DWORD(i));
		sphereIndices.push_back(DWORD(i + 1));
	}

	this->vertexBufferSphere.initialize(this->sphereVector.data(), (UINT)this->sphereVector.size(), System::getDevice());
	this->indexBufferSphere.initialize(this->sphereIndices.data(), (UINT)this->sphereIndices.size(), System::getDevice());
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

void GameObjectHandler::sortBackToFront()
{
	System::theCamera->SetPosition(0, 10, -20);//
	DirectX::XMFLOAT3 camPos=System::theCamera->GetPosition();
	int low = 0;
	for (int i = 0; i < nrOfTrans; i++)
	{
		for (int j = i+1; j < nrOfTrans; j++)
		{
			DirectX::XMFLOAT3 camToModel = DirectX::XMFLOAT3(this->transModels[low].selfPtr->getPosition().x - camPos.x,
				this->transModels[low].selfPtr->getPosition().y - camPos.y,
				this->transModels[low].selfPtr->getPosition().z - camPos.z);

			float length1 = sqrt(pow(camToModel.x, 2) + pow(camToModel.y, 2) + pow(camToModel.z, 2));
			DirectX::XMFLOAT3 camToModel2 = DirectX::XMFLOAT3(this->transModels[j].selfPtr->getPosition().x - camPos.x,
				this->transModels[j].selfPtr->getPosition().y - camPos.y,
				this->transModels[j].selfPtr->getPosition().z - camPos.z);
			float length2 = sqrt(pow(camToModel2.x, 2) + pow(camToModel2.y, 2) + pow(camToModel2.z, 2));
			if (length2 > length1)
			{
				low = j;
			}
		}
		ModWorld temp = this->transModels[i];
		this->transModels[i] = this->transModels[low];
		this->transModels[low] = temp;
	}

}
