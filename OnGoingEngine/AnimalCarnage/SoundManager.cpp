#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() : retryAudio(false), loopedEffect(nullptr)
{
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
	#ifdef _DEBUG
	eflags = eflags | DirectX::AudioEngine_Debug;
	#endif

	try
	{
		this->audioEngine = new DirectX::AudioEngine(eflags);
	}
	catch (const std::exception&)
	{
		this->audioEngine = nullptr;
	}
}

SoundManager::~SoundManager()
{
	if (this->audioEngine)
	{
		this->audioEngine->Suspend();
	}

	delete this->audioEngine;

	for (int i = 0; i < this->effects.size(); i++)
	{
		delete this->effects[i].sound;
	}

	if (this->loopedEffect)
	{
		this->loopInstance.reset();
		delete this->loopedEffect;
	}
}

void SoundManager::update()
{
	if (this->audioEngine)
	{
		if (this->retryAudio)
		{
			this->retryAudio = false;

			if (this->audioEngine->Reset())
			{
				if (this->loopInstance)
				{
					this->loopInstance->Play(true);
				}
			}
		}
		else if (!this->audioEngine->Update())
		{
			if (this->audioEngine->IsCriticalError())
			{
				this->retryAudio = true;
			}
		}
	}
}

void SoundManager::playEffect(std::string id)
{
	for (int i = 0; i < effects.size(); i++)
	{
		if (this->effects[i].id == id)
		{
			this->effects[i].sound->Play();
			break;
		}
	}
}

void SoundManager::loadEffect(std::wstring filename, std::string id)
{
	if (id == "" || !this->audioEngine)
	{
		return;
	}

	bool loaded = false;

	for (int i = 0; i < effects.size(); i++)
	{
		if (this->effects[i].id == id)
		{
			delete this->effects[i].sound;
			this->effects[i].sound = new DirectX::SoundEffect(this->audioEngine, (L"Sound/" + filename).c_str());
			loaded = true;

			break;
		}
	}

	if (!loaded)
	{
		Effect effect;
		effect.id = id;
		effect.sound = new DirectX::SoundEffect(this->audioEngine, (L"Sound/" + filename).c_str());

		this->effects.push_back(effect);
	}
}

void SoundManager::unloadEffect(std::string id)
{
	int index = -1;

	for (int i = 0; i < effects.size() && index == -1; i++)
	{
		if (this->effects[i].id == id)
		{
			index = i;
		}
	}

	if (index != -1)
	{
		delete this->effects[index].sound;
		this->effects.erase(this->effects.begin() + index);
	}
}

void SoundManager::playLooped(std::wstring filename)
{
	if (this->audioEngine)
	{
		if (this->loopedEffect)
		{
			this->loopInstance.reset();
			delete this->loopedEffect;
		}

		//https://github.com/Microsoft/DirectXTK/wiki/SoundEffect <-- Kolla här för att sätta loop-punkter!
		this->loopedEffect = new DirectX::SoundEffect(this->audioEngine, (L"Sound/" + filename).c_str());
		this->loopInstance = this->loopedEffect->CreateInstance();

		this->loopInstance->Play(true);
	}
}

void SoundManager::stopLooped()
{
	if (this->loopedEffect)
	{
		this->loopInstance->Stop(true);
	}
}

void SoundManager::stopAllSounds()
{
	if (this->audioEngine)
	{
		this->audioEngine->Reset();

		if (this->loopedEffect)
		{
			this->loopInstance->Stop(true);
		}
	}
}
