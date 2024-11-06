#include "pch.h"
#include "GameObjectMananger.h"
#include "GameObject.h"
#include "Pole.h"
#include "Character.h"
#include "Box.h"
#include <iostream>
#include "Level.h"
#include"PickUpManager.h"
GameObjectMananger::GameObjectMananger()
	:m_TypeOfObject{TypeOfObject::pole}
	,m_CanBeGrabbed{false}
{
}

GameObjectMananger::~GameObjectMananger()
{
	for (size_t i = 0; i < m_pVectorOfGameObjects.size(); i++)
	{
		delete m_pVectorOfGameObjects[i];
		m_pVectorOfGameObjects[i] = nullptr;
	}
	m_pVectorOfGameObjects.clear();

}

void GameObjectMananger::PushBackNewObject(TypeOfObject type,const Rectf& pos, TextureManager* pTextureManager, float width,float height,PickUpManager* pPickUpManager)
{
	switch (type)
	{
	case GameObjectMananger::TypeOfObject::pole:
		m_pVectorOfGameObjects.push_back(new Pole(pos));
		m_CanBeGrabbed = true;
		break;
	case GameObjectMananger::TypeOfObject::box:
		m_pVectorOfGameObjects.push_back(new Box{ pos,pTextureManager,pPickUpManager });
		m_CanBeGrabbed = false;
		break;
	}
}

void GameObjectMananger::UpdateObjects(float elapsedSec,Level* level, PickUpManager* pPickUpManager)
{
	for (size_t i = 0; i < m_pVectorOfGameObjects.size(); i++)
	{
		if (m_pVectorOfGameObjects[i]->IsFinished())
		{
			delete m_pVectorOfGameObjects[i];
			m_pVectorOfGameObjects[i] = m_pVectorOfGameObjects.back();
			m_pVectorOfGameObjects.pop_back();
			break;
		}
		m_pVectorOfGameObjects[i]->Update(elapsedSec,level,pPickUpManager);
	}
}

void GameObjectMananger::DrawObjects() const
{
	for (size_t i = 0; i < m_pVectorOfGameObjects.size(); i++)
	{
		m_pVectorOfGameObjects[i]->Draw();
	}
}

void GameObjectMananger::ClearGameObjects()
{
	for (size_t i = 0; i < m_pVectorOfGameObjects.size(); i++)
	{
		delete m_pVectorOfGameObjects[i];
		m_pVectorOfGameObjects[i] = nullptr;
	}
	m_pVectorOfGameObjects.clear();

}

const Rectf& GameObjectMananger::GetObjectShape() const
{

	for (size_t i = 0; i < m_pVectorOfGameObjects.size(); ++i)
	{
		return m_pVectorOfGameObjects[i]->GetObjectShape();
	}
	return Rectf{ 0,0,0,0 };

}

bool GameObjectMananger::isFinished()
{
	for (size_t i = 0; i < m_pVectorOfGameObjects.size(); i++)
	{
		return m_pVectorOfGameObjects[i]->IsFinished();
	}
	return false;
}

void GameObjectMananger::HitBox(Character* character)
{
	for (size_t i = 0; i < m_pVectorOfGameObjects.size(); i++)
	{
			m_pVectorOfGameObjects[i]->IsHit(character->GetAttackShape());

	}
}

bool GameObjectMananger::GetCanBeGrabbed()
{
	return m_CanBeGrabbed;
}

const std::vector<GameObject*>& GameObjectMananger::GetObjectVector()
{
	return m_pVectorOfGameObjects;
}

const std::vector<Point2f>& GameObjectMananger::GetPoleVertices()
{
	for (size_t i = 0; i < m_pVectorOfGameObjects.size(); i++)
	{
		return m_pVectorOfGameObjects[i]->GetPoleVertices();
	}
	return std::vector<Point2f>{};
}
