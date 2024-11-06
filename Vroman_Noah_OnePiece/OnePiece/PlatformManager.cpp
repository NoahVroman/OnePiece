#include "pch.h"
#include "PlatformManager.h"

PlatformManager::PlatformManager()
{
}

PlatformManager::~PlatformManager()
{
	for (size_t i = 0; i < m_pVectorOfPlatforms.size(); i++)
	{
		delete m_pVectorOfPlatforms[i];
		m_pVectorOfPlatforms[i] = nullptr;
	}
	m_pVectorOfPlatforms.clear();
}

void PlatformManager::PushBack(const Rectf& shape)
{
	m_pVectorOfPlatforms.push_back(new Platform( shape ));
}

void PlatformManager::ClearPlatforms()
{
	for (size_t i = 0; i < m_pVectorOfPlatforms.size(); i++)
	{
		delete m_pVectorOfPlatforms[i];
		m_pVectorOfPlatforms[i] = nullptr;
	}
	m_pVectorOfPlatforms.clear();

}


void PlatformManager::HandleAllPlatformCollision(Rectf& shapeCharacter, Vector2f& velocityCharacter)
{
	for (size_t i = 0; i < m_pVectorOfPlatforms.size(); ++i)
	{
		m_pVectorOfPlatforms[i]->HandleCollision(shapeCharacter, velocityCharacter);
	}
}

bool PlatformManager::isPlayerOnGround(const Rectf& ShapeAvatar, const Vector2f& VelocityAvatar)const
{
	for (size_t i = 0; i < m_pVectorOfPlatforms.size(); ++i)
	{
		if (m_pVectorOfPlatforms[i]->isOnGround(ShapeAvatar, VelocityAvatar))
		{
			return true;
		}
	}
	return false;
}
