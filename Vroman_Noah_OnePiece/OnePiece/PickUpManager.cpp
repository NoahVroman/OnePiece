#include "pch.h"
#include "PickUpManager.h"
#include "Apple.h"
#include "TextureManager.h"
PickUpManager::PickUpManager()
{
}

PickUpManager::~PickUpManager()
{
	for (size_t i = 0; i < m_pPickups.size(); i++)
	{
		delete m_pPickups[i];
		m_pPickups[i] = nullptr;
	}
	m_pPickups.clear();
}

void PickUpManager::Draw() const
{
	for (size_t i = 0; i < m_pPickups.size(); i++)
	{
		m_pPickups[i]->Draw();
	}

}

void PickUpManager::Update(float elapsedSec)
{
	for (size_t i = 0; i < m_pPickups.size(); i++)
	{
		m_pPickups[i]->Update(elapsedSec);

		if (m_pPickups[i]->IsFinished())
		{
			delete m_pPickups[i];
			m_pPickups[i] = m_pPickups.back();
			m_pPickups.pop_back();
			break;

		}
	}
}

void PickUpManager::AddPickup(const Point2f& pos, TextureManager* pTextureManager)
{
	m_pPickups.push_back(new Apple{pos,pTextureManager});
}

bool PickUpManager::IsOverlapping(const Rectf& shape)
{
	bool isOverlapping = false; // Flag to track if overlap occurs
	for (size_t i = 0; i < m_pPickups.size(); i++)
	{
		if (m_pPickups[i]->IsOverlapping(shape))
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

bool PickUpManager::IsFinished()
{
	for (size_t i = 0; i < m_pPickups.size(); i++)
	{
		if (m_pPickups[i]->IsFinished())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void PickUpManager::ClearPickUpVector()
{
	for (size_t i = 0; i < m_pPickups.size(); i++)
	{
		delete m_pPickups[i];
		m_pPickups[i] = nullptr;
	}
	m_pPickups.clear();

}

const size_t& PickUpManager::GetPickUpCount() const
{
	return m_pPickups.size();
}

Apple* PickUpManager::GetPickup(const size_t& index)
{
	if (index < m_pPickups.size())
	{
		return m_pPickups[index];
	}
	else
	{
		return nullptr;
	}
}
