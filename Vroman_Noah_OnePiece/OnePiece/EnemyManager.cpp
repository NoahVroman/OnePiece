#include "pch.h"
#include "EnemyManager.h"
#include "BaseEnemyClass.h"
#include "Level.h"
#include "Character.h"
#include "Pirate.h"
#include "AxeMorgan.h"
#include "Bomber.h"
#include <iostream>
#include "HUD.h"
#include "SoundManager.h"
#include"ProjectileManager.h"

EnemyManager::EnemyManager()
    :m_EnemyType{Type::pirate}
{
}

EnemyManager::~EnemyManager()
{
    for (size_t i = 0; i < m_pVectorOfEnemies.size(); i++)
    {
        delete m_pVectorOfEnemies[i];
        m_pVectorOfEnemies[i] = nullptr;
    }
    m_pVectorOfEnemies.clear();
}

void EnemyManager::PushBackNewEnemy(const Type& typeOfEnemy, TextureManager* textureMananger, const Point2f& pos, ProjectileManager* pProjectileManager, SoundManager* pSoundManager)
{


    switch (typeOfEnemy)
    {
    case EnemyManager::Type::pirate:
         m_pVectorOfEnemies.push_back(new Pirate(pos, textureMananger,pProjectileManager,pSoundManager));
        break;
    case EnemyManager::Type::boss:
        m_pVectorOfEnemies.push_back(new AxeMorgan(pos,textureMananger, pProjectileManager,pSoundManager));
        break;
    case EnemyManager::Type::bomber:
        m_pVectorOfEnemies.push_back(new Bomber(pos, textureMananger, pProjectileManager,pSoundManager));
        break;
    }


}

void EnemyManager::UpdateAllEnemies(float elapsedSec, Level* level, Character* character,HUD* hud)
{
    for (size_t index = 0; index < m_pVectorOfEnemies.size(); ++index)
    {
        	
        if (m_pVectorOfEnemies[index]->IsFinished())
        {
             delete m_pVectorOfEnemies[index];
             m_pVectorOfEnemies[index] = m_pVectorOfEnemies.back();
             m_pVectorOfEnemies.pop_back();
             break;
        }

        BaseEnemyClass* pCurrentEnemy = m_pVectorOfEnemies[index];
        pCurrentEnemy->Update(elapsedSec, level, character,hud);

        pCurrentEnemy->IsHit(character->GetAttackShape(), character->GetDamage());

        if (pCurrentEnemy->HitCharacter(character->GetPlayerShape()))
        {
            character->IsHit(pCurrentEnemy->GetPointOfInpact(), pCurrentEnemy->GetDamage(),elapsedSec);
        }

    }

}

void EnemyManager::DrawAllEnemies() const
{
    for (size_t index = 0; index < m_pVectorOfEnemies.size(); index++)
    {
       m_pVectorOfEnemies[index]->Draw();
    }

}

void EnemyManager::ClearEnemies()
{
    for (size_t i = 0; i < m_pVectorOfEnemies.size(); i++)
    {
        delete m_pVectorOfEnemies[i];
        m_pVectorOfEnemies[i] = nullptr;
    }
    m_pVectorOfEnemies.clear();
}

void EnemyManager::PopEnemies()
{
}

EnemyManager::Type EnemyManager::GetTypeOfEnemy()
{
    return m_EnemyType;
}

const Rectf& EnemyManager::GetShapeOfEnemy()
{
    for (size_t i = 0; i < m_pVectorOfEnemies.size(); i++)
    {
        return m_pVectorOfEnemies[i]->GetActorShape();
    }
    return Rectf{ 0,0,0,0 };
}

AxeMorgan* EnemyManager::GetAxeMorgan()
{
    for (size_t i = 0; i < m_pVectorOfEnemies.size(); i++)
    {
        AxeMorgan* axeMorganEnemy = dynamic_cast<AxeMorgan*>(m_pVectorOfEnemies[i]);
        if (axeMorganEnemy != nullptr)
        {
            return axeMorganEnemy;
        }
    }
    return nullptr;
}

