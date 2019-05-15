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
	
	//System::theGraphicDevice->setRasterFront();
	System::theGraphicDevice->setRasterState();

	//System::theGraphicDevice->setRasterFront();
	System::shaderManager->getShadowMapping()->prepare();//setshader + omsetrendertarget(0,0,depthstencilview
	DirectX::XMVECTOR lightDirView = DirectX::XMVectorSet(0, 0, 0, 1);
	float lightViewLengt = 7;
	DirectX::XMVECTOR CamPos = DirectX::XMVectorSet(lightViewLengt * (-1 * this->lightsCB.data.lights[0].direction[0]), lightViewLengt * (-1 * this->lightsCB.data.lights[0].direction[1]), lightViewLengt * (this->lightsCB.data.lights[0].direction[2]), 1);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);

		//this->lightsCB.data.lights[2].position
	

		

	//this->lightsCB.data.lights[2].position
	// Variables you already know:
	DirectX::XMVECTOR lightDirectionVector = DirectX::XMVectorSet(this->lightsCB.data.lights[0].direction[0], this->lightsCB.data.lights[0].direction[1], this->lightsCB.data.lights[0].direction[1], 1); // the light direction
	// Variables you have to define somehow:
	DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0,0,0,1);



	System::shaderManager->getShadowMapping()->setCBuffers();
	//DirectX::XMMatrixLookAtLH(CamPos, lightDirView, up);
	System::shaderManager->getShadowMapping()->setView(DirectX::XMMatrixLookAtLH(CamPos, lightDirView, up));
	for (int i = 0; i < this->nrOfOpaque; i++)
	{
		if (i != 3)
		{
			System::shaderManager->getShadowMapping()->setWorld(*this->opaqueModels[i].worldPtr);
			this->opaqueModels[i].modelPtr->drawOnlyVertex();
		}
	}
	this->lightsCB.data.nrOfLights = nrOfLights;
	this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
	DirectX::XMMATRIX worldPos = DirectX::XMMatrixTranslation(gameObjects[6]->getPosition().x, gameObjects[6]->getPosition().y+2, gameObjects[6]->getPosition().z);
	this->lightsCB.data.lights[1].worldLight = worldPos;
	//this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
	float color[] = {
		0,0,0,1.0f
	};
	System::theGraphicDevice->setViewPort();
	System::theGraphicDevice->setRasterState();
	float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	System::getDeviceContext()->OMSetBlendState(nullptr, blendFactor, 0xffffffff);
	//System::theGraphicDevice->turnOnZ();
	System::shaderManager->getDefShader()->setCBuffers();
	//System::shaderManager->getDefShader()->setConstanbuffer(PIXEL, 1, this->lightsCB.getBuffer());
	System::shaderManager->getDefShader()->setShaders();
	System::shaderManager->getDefShader()->prepGBuffer(color);

	for (int i = 0; i < this->nrOfOpaque; i++)
	{
		this->opaqueModels[i].modelPtr->getShader()->setWorld(*this->opaqueModels[i].worldPtr);
		this->opaqueModels[i].modelPtr->draw();
	}
	System::shaderManager->getDefShader()->prepForLight();
	System::theGraphicDevice->setBackBuffer(System::shaderManager->getDefShader()->gBuffer.getDepthStcView());
	//
	//System::shaderManager->getDefShader()->resetCB();
	System::shaderManager->getLightShader()->setCBuffers();
	System::shaderManager->getLightShader()->setConstanbuffer(PIXEL, 1, this->lightsCB.getBuffer());
	//System::getDeviceContext()->OMSetRenderTargets()
	System::getDeviceContext()->PSSetShaderResources(3, 1, &System::shaderManager->getShadowMapping()->getShadowMap());
	System::shaderManager->getShadowMapping()->setPSDepthView();//sets the mvp for the depth in lspixelshader
	System::shaderManager->getLightShader()->setShaders();
	System::theGraphicDevice->turnOffZ();

	UINT32 offset = 0; 	
	//this->lightsCB.data.index = 0;
	//this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
	System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBufferQuad.GetAddressOf(), &*vertexBufferQuad.getStridePtr(), &offset);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	System::shaderManager->getLightShader()->setTypeOfLight(0);//0 directlight
	this->lightsCB.data.index = 0;
	this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
	System::shaderManager->getLightShader()->renderShaderDir((int)quad.size());//render fullscreen quad with all lights
	UINT32 offsetS = 0;
	System::getDeviceContext()->IASetIndexBuffer(indexBufferSphere.getBuffer(), DXGI_FORMAT_R32_UINT, 0);
	System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBufferSphere.GetAddressOf(), &*vertexBufferSphere.getStridePtr(), &offsetS);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	System::shaderManager->getLightShader()->setTypeOfLight(1);//1 pointlight

	for (int i = 1; i < nrOfLights; i++)
	{
		DirectX::XMMATRIX worldSphere= DirectX::XMMatrixTranspose(XMMatrixScaling(this->lightsCB.data.lights[i].position[3]*10, this->lightsCB.data.lights[i].position[3]*10, this->lightsCB.data.lights[i].position[3]*10)*
			this->lightsCB.data.lights[i].worldLight);
		this->lightsCB.data.index = i;
		this->lightsCB.applyChanges(System::getDevice(), System::getDeviceContext());
		System::shaderManager->getLightShader()->setWorld(worldSphere);
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
	
	System::shaderManager->getForwardShader()->setShaders();//tänker att man kör denna sen renderar allla som använder denna shader sen tar setshader på nästa osv.
	System::shaderManager->getForwardShader()->setCBuffers();
	System::shaderManager->getForwardShader()->setConstanbuffer(PIXEL, 1, this->lightsCB.getBuffer());
	/*for (int i = 0; i < this->nrOfOpaque; i++)
	{
		this->opaqueModels[i].modelPtr->getShader()->setWorld(*this->opaqueModels[i].worldPtr);
		this->opaqueModels[i].modelPtr->draw();
	}*/
	
	////Forward
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

void GameObjectHandler::setSkyboxTexture(std::string file)
{
	System::skybox->setTexture(file);
}

void GameObjectHandler::generateSphere()
{
	double latitudeBands = 5;
	double longitudeBands = 5;
	double radius = 0.5f;

	//for (double latNumber = 0; latNumber <= latitudeBands; latNumber++)
	//{
	//	double theta = latNumber * M_PI / latitudeBands;
	//	double sinTheta = sin(theta);
	//	double cosTheta = cos(theta);

	//	for (double longNumber = 0; longNumber <= longitudeBands; longNumber++)
	//	{
	//		double phi = longNumber * 2 * M_PI / longitudeBands;
	//		double sinPhi = sin(phi);
	//		double cosPhi = cos(phi);

	//		LightVertex vs;
	//		float Normal[3] = {
	//			0,0,0	
	//		};
	//		Normal[0] = cosPhi * sinTheta;   // x
	//		Normal[1] = cosTheta;            // y
	//		Normal[2] = sinPhi * sinTheta;   // z
	//		vs.uv[0] = 1 - (longNumber / longitudeBands); // u
	//		vs.uv[1] = 1 - (latNumber / latitudeBands);   // v
	//		vs.position[0] = radius * Normal[0];   //x
	//		vs.position[1] = radius * Normal[1];   //y
	//		vs.position[2] = radius * Normal[2];   //z

	//		sphereVector.push_back(vs);
	//	}

	//	for (int latNumber = 0; latNumber < latitudeBands; latNumber++)
	//	{
	//		for (int longNumber = 0; longNumber < longitudeBands; longNumber++) 
	//		{
	//			int first = (latNumber * (longitudeBands + 1)) + longNumber;
	//			int second = first + longitudeBands + 1;

	//			sphereIndices.push_back(first);
	//			sphereIndices.push_back(second);
	//			sphereIndices.push_back(first + 1);

	//			sphereIndices.push_back(second);
	//			sphereIndices.push_back(second + 1);
	//			sphereIndices.push_back(first + 1);

	//		}
	//	}
	//}




	/*int n;

	double a;

	double b;

	double R = 0.5;
	double H = 0;
	double K = 0;
	double Z = 0;

	n = 0;

	const int space = 1;

	LightVertex vertex;
	for (b = 0; b <= 90 - space; b += space) {

		for (a = 0; a <= 360 - space; a += space) {



			vertex.position[0] = R * sin((a) / 180 * M_PI) * sin((b) / 180 * M_PI) - H;

			vertex.position[1] = R * cos((a) / 180 * M_PI) * sin((b) / 180 * M_PI) + K;

			vertex.position[2] = R * cos((b) / 180 * M_PI) - Z;

			vertex.uv[0] = (2 * b) / 360;

			vertex.uv[1] = (a) / 360;

			n++;
			sphereVector.push_back(vertex);



			vertex.position[0] = R * sin((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI

			) - H;

			vertex.position[1] = R * cos((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI

			) + K;

			vertex.position[2] = R * cos((b + space) / 180 * M_PI) - Z;

			vertex.uv[0] = (2 * (b + space)) / 360;

			vertex.uv[1] = (a) / 360;

			n++;
			sphereVector.push_back(vertex);


			vertex.position[0] = R * sin((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI

			) - H;

			vertex.position[1] = R * cos((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI

			) + K;

			vertex.position[2] = R * cos((b) / 180 * M_PI) - Z;

			vertex.uv[0] = (2 * b) / 360;

			vertex.uv[1] = (a + space) / 360;

			n++;
			sphereVector.push_back(vertex);


			vertex.position[0] = R * sin((a + space) / 180 * M_PI) * sin((b + space) /

				180 * M_PI) - H;

			vertex.position[1] = R * cos((a + space) / 180 * M_PI) * sin((b + space) /

				180 * M_PI) + K;

			vertex.position[2] = R * cos((b + space) / 180 * M_PI) - Z;

			vertex.uv[0] = (2 * (b + space)) / 360;

			vertex.uv[1] = (a + space) / 360;

			n++;
			sphereVector.push_back(vertex);


		}

	}*/


	int stacks = 20;
	int slices = 20;
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
