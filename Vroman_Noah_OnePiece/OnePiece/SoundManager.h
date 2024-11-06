#pragma once
#include <unordered_map>
class SoundStream;
class SoundEffect;

class SoundManager final
{
public:

	SoundManager();
	~SoundManager();

	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;

	
	SoundStream* GetSoundStreamPointer(const std::string& filename);
	SoundEffect* GetSoundEffectPointer(const std::string& filename);

	void PauseAll();
	void ResumeAll();

	void IncreaseStreamVolume();
	void IncreaseEffectsVolume();

	void DecreaseStreamVolume();
	void DecreaseEffectsVolume();

	void MuteAndUnmute();

	const int GetStreamsVolume() const;
	const int GetEffectsVolume()const;

private:
	std::unordered_map<std::string, SoundStream*> m_SoundStreamMap;
	std::unordered_map<std::string, SoundEffect*> m_SoundEffectsMap;

	int m_StreamsVolume; 
	int m_EffectsVolume;

	bool m_IsMuted;
};

