#include "pch.h"
#include "ProjectileManager.h"
#include "Blade.h"
#include <iostream>
#include "Bomb.h"
#include "Level.h"
#include "SoundManager.h"
ProjectileManager::ProjectileManager()
{

}
ProjectileManager::~ProjectileManager()
{
	for (size_t i = 0; i < m_pProjectileVector.size(); i++)
	{
		delete m_pProjectileVector[i];
		m_pProjectileVector[i] = nullptr;
	}
	m_pProjectileVector.clear();
}

void ProjectileManager::AddProjectile(const Type& type,const Point2f& position, TextureManager* pTextureManager, const Vector2f& velocity, SoundManager* pSoundManager)
{
	switch (type)
	{
	case ProjectileManager::Type::blade:
		m_pProjectileVector.push_back(new Blade(position, pTextureManager, velocity));
		break;
	case ProjectileManager::Type::bomb:
		m_pProjectileVector.push_back(new Bomb(position, pTextureManager, velocity,pSoundManager));
		break;
	}
}

void ProjectileManager::PopProjectile()
{
	for (size_t i = 0; i < m_pProjectileVector.size(); i++)
	{
			delete m_pProjectileVector[i];
			m_pProjectileVector[i] = m_pProjectileVector.back();
			m_pProjectileVector.pop_back();
			break;

	}
}

void ProjectileManager::ClearProjectile()
{
	for (size_t i = 0; i < m_pProjectileVector.size(); i++)
	{
		delete m_pProjectileVector[i];
		m_pProjectileVector[i] = nullptr;
	}
	m_pProjectileVector.clear();

}

void ProjectileManager::Draw() const
{
	for (size_t i = 0; i < m_pProjectileVector.size(); i++)
	{
		m_pProjectileVector[i]->Draw();
	}
}

void ProjectileManager::Update(float elapsedSec,Level* level, const Rectf& characterShape)
{
	for (size_t i = 0; i < m_pProjectileVector.size(); i++)
	{
		m_pProjectileVector[i]->Update(elapsedSec,level,characterShape);
	}

}

bool ProjectileManager::IsOverlapping(const Rectf& shape)
{
	bool isOverlapping = false; // Flag to track if overlap occurs
	for (size_t i = 0; i < m_pProjectileVector.size(); i++)
	{
		if (m_pProjectileVector[i]->IsOverlapping(shape))
		{
			if (!isOverlapping) // Only set to true if it's not already overlapping
			{
				isOverlapping = true;
				break;
			}
		}
	}
	return isOverlapping;
}

const Rectf& ProjectileManager::GetProjectileShape()
{
	for (size_t i = 0; i < m_pProjectileVector.size(); i++)
	{
		return m_pProjectileVector[i]->GetShape();
	}
	return Rectf{0,0,0,0};
}

int ProjectileManager::GetDamage() const
{
	for (size_t i = 0; i < m_pProjectileVector.size(); i++)
	{
		return m_pProjectileVector[i]->GetDamage();
	}
	return 0;
}

bool ProjectileManager::GetFinished() const
{
	for (size_t i = 0; i < m_pProjectileVector.size(); i++)
	{
		return m_pProjectileVector[i]->GetIsFinished();
	}

	return false;
}
