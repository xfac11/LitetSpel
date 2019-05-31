#include "ParticleManager.h"
#include "SimpleEffect.h"

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

void ParticleManager::addSimpleEffect(SimpleMath::Vector3 position, std::string filename,float size,float lifetime,bool gravity,int particleCount,float maxStartSpeed)
{
	SimpleEffect* newEffect = new SimpleEffect(position, lifetime,gravity, particleCount,size, maxStartSpeed, filename);
	this->activeEffects.push_back(newEffect);
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