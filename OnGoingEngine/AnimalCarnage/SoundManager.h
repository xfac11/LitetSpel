#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include <vector>
#include "Audio.h"

#define SOUND_EFFECT_CAP 32

enum Priority
{
	LOWEST,
	LOWER,
	NORMAL,
	HIGH,
	HIGHEST
};

class SoundManager
{
private:
	struct Effect
	{
		int priorty;
		std::string id;
		DirectX::SoundEffect* sound;
	} effects[SOUND_EFFECT_CAP];

	DirectX::SoundEffect* loopedEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> loopInstance;

	DirectX::AudioEngine* audioEngine;
	bool retryAudio;

public:
	SoundManager();
	virtual ~SoundManager();

	void update();

	void playEffect(std::string id);
	void loadEffect(std::wstring filename, std::string id, Priority prio);
	void unloadEffect(std::string id);

	void playLooped(std::wstring filename);
	void stopLooped();

	void stopAllSounds();
};

#endif // !SOUND_MANAGER_H
