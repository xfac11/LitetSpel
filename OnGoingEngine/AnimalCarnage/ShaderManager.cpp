#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	this->fShader = nullptr;
	this->dShader = nullptr;
	this->lShader = nullptr;
	this->shadowMapping = nullptr;
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
	if (this->horBlur != nullptr)
		delete this->horBlur;
	if (this->verBlur != nullptr)
		delete this->verBlur;
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
	this->horBlur = new HorizontalBlur;
	this->horBlur->initialize(height, width);
	this->verBlur = new VerticalBlur;
	this->verBlur->initialize(height, width);
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

HorizontalBlur *& ShaderManager::getHorBlur()
{
	return this->horBlur;
}

VerticalBlur *& ShaderManager::getVerBlur()
{
	return this->verBlur;
}
