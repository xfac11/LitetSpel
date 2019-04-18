#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	this->fShader = nullptr;
}

ShaderManager::~ShaderManager()
{
	if(this->fShader!=nullptr)
		delete this->fShader;
}

void ShaderManager::initialize()
{
	this->fShader = new ForwardShader;
	this->fShader->initialize();
}

ForwardShader *& ShaderManager::getForwardShader()
{
	return this->fShader;
}
