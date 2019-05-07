#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include <vector>
#include "Audio.h"

class SoundManager
{
private:
	struct Effect
	{
		std::string id;
		DirectX::SoundEffect* sound;
	};

	std::vector<Effect> effects;
	DirectX::SoundEffect* loopedEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> loopInstance;

	DirectX::AudioEngine* audioEngine;
	bool retryAudio;

public:
	SoundManager();
	virtual ~SoundManager();

	void update();

	void playEffect(std::string id);
	void loadEffect(std::wstring filename, std::string id);
	void unloadEffect(std::string id);

	void playLooped(std::wstring filename);
	void stopLooped();

	void stopAllSounds();

	float getMasterVolume() const;
	void setMasterVolume(float volume);
};

#endif // !SOUND_MANAGER_H
