#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	this->fShader = nullptr;
	this->dShader = nullptr;
	this->lShader = nullptr;
}

ShaderManager::~ShaderManager()
{
	if(this->fShader!=nullptr)
		delete this->fShader;
	if (this->dShader != nullptr)
		delete this->dShader;
	if (this->lShader != nullptr)
		delete this->lShader;
	if (this->shadowMapping != nullptr)
		delete this->shadowMapping;
}

void ShaderManager::initialize(int height, int width, float nearPlane, float farPlane)
{
	this->fShader = new ForwardShader;
	this->fShader->initialize();
	this->dShader = new DeferredShader;
	this->dShader->initialize(height, width, nearPlane, farPlane);
	this->lShader = new LightShader;
	this->lShader->initialize();
	this->shadowMapping = new ShadowMapping;
	this->shadowMapping->initialize();
}

ForwardShader *& ShaderManager::getForwardShader()
{
	return this->fShader;
}

DeferredShader *& ShaderManager::getDefShader()
{
	return this->dShader;
}

LightShader *& ShaderManager::getLightShader()
{
	return this->lShader;
}

ShadowMapping *& ShaderManager::getShadowMapping()
{
	return this->shadowMapping;
}
