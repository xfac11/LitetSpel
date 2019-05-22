#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
	for (int i = 0; i < this->activeEffects.size(); i++)
	{
		delete this->activeEffects[i];
	}
}

void ParticleManager::addEffect(SimpleMath::Vector3 position)
{
}

void ParticleManager::resetAndClear()
{
	for (int i = 0; i < this->activeEffects.size(); i++)
	{
		delete this->activeEffects[i];
	}

	this->activeEffects.clear();
}

void ParticleManager::render()
{
	for (int i = 0; i < this->activeEffects.size(); i++)
	{
		this->activeEffects[i]->render();
	}
}

void ParticleManager::update(float deltaTime)
{
	for (int i = 0; i < this->activeEffects.size(); i++)
	{
		if (this->activeEffects[i]->isAlive())
		{
			this->activeEffects[i]->update(deltaTime);
		}
		else
		{
			delete this->activeEffects[i];
			this->activeEffects.erase(this->activeEffects.begin() + i);
		}
	}
}