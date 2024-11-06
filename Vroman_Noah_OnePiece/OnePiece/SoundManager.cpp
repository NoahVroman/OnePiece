#include "pch.h"
#include "SoundManager.h"
#include <SoundStream.h>
#include <SoundEffect.h>

SoundManager::SoundManager()
	:m_StreamsVolume{5}
	,m_EffectsVolume{10}
	,m_IsMuted{false}
{

}

SoundManager::~SoundManager()
{
	for (const std::pair<std::string, SoundStream*>& pair : m_SoundStreamMap)
	{
		delete pair.second;
	}
	m_SoundStreamMap.clear();

	for (const std::pair<std::string, SoundEffect*>& pair : m_SoundEffectsMap)
	{
		delete pair.second;
	}
	m_SoundEffectsMap.clear();
}

SoundStream* SoundManager::GetSoundStreamPointer(const std::string& filename)
{
	SoundStream* pStream{ m_SoundStreamMap[filename] };
	if (!pStream)
	{
		pStream = m_SoundStreamMap[filename] = new SoundStream(filename);

		if (m_IsMuted == true)
		{
			pStream->SetVolume(0);
		}
		else
		{
			pStream->SetVolume(m_StreamsVolume);

		}
	
	}
	return pStream;
}

SoundEffect* SoundManager::GetSoundEffectPointer(const std::string& filename)
{
	SoundEffect* pEffect{ m_SoundEffectsMap[filename] };
	if (!pEffect) 
	{
		pEffect = m_SoundEffectsMap[filename] = new SoundEffect(filename);

		if (m_IsMuted == true)
		{
			pEffect->SetVolume(0);
		}
		else
		{
			pEffect->SetVolume(m_EffectsVolume);

		}
	}
	return pEffect;	
}

void SoundManager::PauseAll()
{
	for (const std::pair<std::string, SoundStream*>& pair : m_SoundStreamMap)
	{
		pair.second->Pause();
	}

	for (const std::pair<std::string, SoundEffect*>& pair : m_SoundEffectsMap)
	{
		pair.second->PauseAll();
	}

}

void SoundManager::ResumeAll()
{
	for (const std::pair<std::string, SoundStream*>& pair : m_SoundStreamMap)
	{
		pair.second->Resume();
	}

	for (const std::pair<std::string, SoundEffect*>& pair : m_SoundEffectsMap)
	{
		pair.second->ResumeAll();
	}

}

void SoundManager::IncreaseStreamVolume()
{
	if (!m_IsMuted)
	{
		for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
		{
			int currentVolume{ mappedElement.second->GetVolume() };
			if (currentVolume < 128)
			{
				mappedElement.second->SetVolume(currentVolume + 4);

			}
		}
		if (m_StreamsVolume < 128)
		{
			m_StreamsVolume += 4;
		}
	}
}

void SoundManager::IncreaseEffectsVolume()
{
	if (!m_IsMuted)
	{
		for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectsMap)
		{
			int currentVolume{ mappedElement.second->GetVolume() };
			if (currentVolume < 128)
			{
				mappedElement.second->SetVolume(currentVolume + 4);

			}
		}
		if (m_EffectsVolume < 128)
		{
			m_EffectsVolume += 4;
		}
	}

}

void SoundManager::DecreaseStreamVolume()
{
	if (!m_IsMuted)
	{
		for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
		{
			int currentVolume{ mappedElement.second->GetVolume() };
			if (currentVolume > 0)
			{
				mappedElement.second->SetVolume(currentVolume - 4);

			}
		}
		if (m_StreamsVolume > 128)
		{
			m_StreamsVolume -= 4;
		}
	}

}

void SoundManager::DecreaseEffectsVolume()
{
	if (!m_IsMuted)
	{
		for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectsMap)
		{
			int currentVolume{ mappedElement.second->GetVolume() };
			if (currentVolume > 0)
			{
				mappedElement.second->SetVolume(currentVolume - 4);

			}
		}
		if (m_EffectsVolume > 128)
		{
			m_EffectsVolume -= 4;
		}
	}

}

void SoundManager::MuteAndUnmute()
{
	m_IsMuted = !m_IsMuted;
	if (m_IsMuted)
	{
		for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
		{
			mappedElement.second->SetVolume(0);

		}

		for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectsMap)
		{
			mappedElement.second->SetVolume(0);

		}
	}
	else
	{
		for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
		{
			mappedElement.second->SetVolume(m_StreamsVolume);

		}

		for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectsMap)
		{
			mappedElement.second->SetVolume(m_EffectsVolume);

		}
	}
}

const int SoundManager::GetStreamsVolume() const
{
	return m_StreamsVolume;
}

const int SoundManager::GetEffectsVolume() const
{
	return m_EffectsVolume;
}
