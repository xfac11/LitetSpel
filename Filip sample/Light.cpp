#include "Light.h"
Light::Light()
{
	this->light = nullptr;
	this->cap = 5;
	this->nrOfLights = 0;
	this->light = new (std::nothrow)AnyLight[5];
	
}

Light::~Light()
{
	
}

void Light::shutdown()
{
	delete[] this->light;//aligned free?
	for (int i = 0; i < 3; i++)
	{
		this->modelsVolumeLights[i].shutdown();
	}
}

void Light::initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	this->modelsVolumeLights[0] = Model();
	this->modelsVolumeLights[0].loadOBJ("sphere.obj", "smooth_32.tga", device, deviceContext);
	this->modelsVolumeLights[0].createTheVertexBuffer(device);
	this->modelsVolumeLights[1] = Model();
	//this->modelsVolumeLights[1].loadOBJ("quad.obj", "flat", device, deviceContext);// Does not have model for Spot and dirLight
	this->modelsVolumeLights[2] = Model();
	//this->modelsVolumeLights[2].loadOBJ("cone.obj", "flat", device, deviceContext);// Does not have model for Spot and dirLight
}

float Light::calcRadius()
{
	float radius;
	float constant = 1.0f;
	float linear = 0.7f;
	float quadratic = 1.8f;

	for (int i = 0; i < nrOfLights; i++)
	{
		float lightMax = std::fmaxf(std::fmaxf(light[i].color.x, light[i].color.y), light[i].color.z);
		radius =
			(-linear + std::sqrtf(linear*linear - 4 * quadratic*(constant - (256.0 / 5.0)*lightMax)))
			/ (2 * quadratic);
		light[i].position.w = radius;
	}
	
	return radius;
}

void Light::addLight(LightTypes type, DirectX::XMFLOAT4 color, DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 direction)
{
	if (this->nrOfLights == cap)
	{
		this->cap += 5;
		AnyLight* temp = new (std::nothrow)AnyLight[this->cap];
		for (int i = 0; i < this->nrOfLights; i++)
		{
			temp[i] = this->light[i];
		}
		delete[] this->light;
		this->light = temp;
	}
	if (type == POINTLIGHT)
	{
		light[nrOfLights].color = color;
		light[nrOfLights].position = position;
		light[nrOfLights].direction.w = float(type);
	}
	else if (type == SPOTLIGHT)
	{
		light[nrOfLights].color = color;
		light[nrOfLights].position = position;
		light[nrOfLights].direction = direction;
		light[nrOfLights].direction.w = float(type);//the w value is not used so we can put the type there
	}
	else if (type == DIRECTIONALLIGHT)
	{
		light[nrOfLights].color = color;
		light[nrOfLights].position = position;//not used for directional light
		light[nrOfLights].direction = direction;
		light[nrOfLights].direction.w = float(type);//the w value is not used so we can put the type there
	}
	this->nrOfLights++;
}

void Light::drawLights(LightShader * shader, ID3D11DepthStencilView* view, ID3D11Device * device, ID3D11DeviceContext * deviceContext,DirectX::XMMATRIX viewMatrix,
	DirectX::XMMATRIX proj,DirectX::XMFLOAT3 camPos,int choice)
{
	for (int i = 0; i < nrOfLights; i++)
	{

		//when direction light it should not scale or set world. the quad will always be the entire screen
		this->modelsVolumeLights[int(light[i].direction.w)].setScale(light[i].position.w, light[i].position.w, light[i].position.w);//direction.w holds the type of light
		this->modelsVolumeLights[int(light[i].direction.w)].setPosition(light[i].position.x, light[i].position.y, light[i].position.z);
		this->modelsVolumeLights[int(light[i].direction.w)].setWorld();


		shader->SetShaderParameters(deviceContext, DirectX::XMLoadFloat4x4(&this->modelsVolumeLights[int(light[i].direction.w)].getId()), viewMatrix, proj, this->light[i], camPos, choice);
		this->modelsVolumeLights[int(light[i].direction.w)].setVertexOnly(deviceContext);
		//Maybe shader.Render so the worldmatrix and all that updates everytime a new object gets drawn?
		shader->render(deviceContext, this->modelsVolumeLights[int(light[i].direction.w)].getVertexCount(), view);
	}
	
}

void Light::moveLight(int index, float x, float y, float z)
{
	this->light[index].position.x += x;
	this->light[index].position.y += y;
	this->light[index].position.z += z;
}

void Light::setPosLight(int index, float x, float y, float z)
{
	this->light[index].position.x = x;
	this->light[index].position.y = y;
	this->light[index].position.z = z;
}
