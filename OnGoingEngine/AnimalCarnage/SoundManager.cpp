#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager()
{
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
	#ifdef _DEBUG
	eflags = eflags | DirectX::AudioEngine_Debug;
	#endif

	this->loopedEffect = nullptr;
	//this->audioEngine = new DirectX::AudioEngine(eflags);
	this->retryAudio = false;

	for (int i = 0; i < SOUND_EFFECT_CAP; i++)
	{
		this->effects[i].priorty = 0;
		this->effects[i].id = "";
		this->effects[i].sound = nullptr;
	}
}

SoundManager::~SoundManager()
{
	//this->audioEngine->Suspend();
	delete this->audioEngine;

	for (int i = 0; i < SOUND_EFFECT_CAP; i++)
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
	/*if (this->retryAudio)
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
	}*/
}

void SoundManager::playEffect(std::string id)
{
	for (int i = 0; i < SOUND_EFFECT_CAP; i++)
	{
		if (this->effects[i].id == id)
		{
			this->effects[i].sound->Play();
		}
	}
}

void SoundManager::loadEffect(std::wstring filename, std::string id, Priority prio)
{
	if (id == "")
	{
		return;
	}

	bool loaded = false;

	for (int i = 0; i < SOUND_EFFECT_CAP && !loaded; i++)
	{
		if (this->effects[i].id == "")
		{
			this->effects[i].priorty = prio;
			this->effects[i].id = id;
			this->effects[i].sound = new DirectX::SoundEffect(this->audioEngine, (L"Sound/" + filename).c_str());
		}
	}

	if (!loaded)
	{
		int lowest = 0;

		for (int i = 0; i < SOUND_EFFECT_CAP && !loaded; i++)
		{
			if (this->effects[i].priorty < this->effects[lowest].priorty)
			{
				lowest = i;
			}
		}

		if (this->effects[lowest].priorty <= prio)
		{
			delete this->effects[lowest].sound;

			this->effects[lowest].priorty = prio;
			this->effects[lowest].id = id;
			this->effects[lowest].sound = new DirectX::SoundEffect(this->audioEngine, (L"Sound/" + filename).c_str());
		}
		else
		{
			std::cout << "[SOUND_MANAGER] Effect " << filename.c_str() << " was not loaded. List full." << std::endl;
		}
	}
}

void SoundManager::unloadEffect(std::string id)
{
	for (int i = 0; i < SOUND_EFFECT_CAP; i++)
	{
		if (this->effects[i].id == id)
		{
			delete this->effects[i].sound;

			this->effects[i].priorty = 0;
			this->effects[i].id = "";
			this->effects[i].sound = nullptr;
			break;
		}
	}
}

void SoundManager::playLooped(std::wstring filename)
{
	if (this->loopedEffect)
	{
		this->loopInstance.reset();
		delete this->loopedEffect;
	}

	//https://github.com/Microsoft/DirectXTK/wiki/SoundEffect <-- Kolla här för att sätta loop punkter!
	this->loopedEffect = new DirectX::SoundEffect(this->audioEngine, (L"Sound/" + filename).c_str());
	this->loopInstance = this->loopedEffect->CreateInstance();

	this->loopInstance->Play(true);
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
	this->audioEngine->Reset();
}
