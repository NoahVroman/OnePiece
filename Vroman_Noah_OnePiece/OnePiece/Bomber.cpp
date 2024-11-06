#include "pch.h"
#include "Bomber.h"
#include "TextureManager.h"
#include "ProjectileManager.h"
#include "Sprite.h"
#include "Character.h"
#include <utils.h>
#include "SoundManager.h"
#include <SoundEffect.h>
#include <iostream>
Bomber::Bomber(const Point2f& pos, TextureManager* pTextureManager, ProjectileManager* pProjectileManager,SoundManager* pSoundManager)
	:BaseEnemyClass(pTextureManager,pSoundManager)
	,m_BomberState{BomberState::idle}
	, m_IsIdlePlayingBackwards{ false }
	,m_HasAttacked{false}
	,m_Timer{0.0f}
	,m_MaxAttackCooldownTimer{3.f}
	,m_OriginalMaximumAttackingTimer{3.f}
	,m_HasThrownBomb{ false }
	
	
{
	const float scale{ 4.f };
	m_pBomberIdleSprite = new Sprite("BomberIdle.png", 7, 1, 7, Point2f{ 0,0 }, scale, pTextureManager);
	m_pBomberAttack = new Sprite("BomberAttack.png", 7, 1, 7, Point2f{ 0,0 }, scale, pTextureManager);
	m_pBomberDeath = new Sprite("BomberDeath.png", 2, 1, 4, Point2f{ 0,0 }, scale, pTextureManager);

	m_pSoundManager = pSoundManager;


	m_BomberShape.left = pos.x;
	m_BomberShape.bottom = pos.y;
	m_BomberShape.width = 348.f;
	m_BomberShape.height = 304.f;

	m_pTextureManager = pTextureManager;
	m_pProjectileManger = pProjectileManager;
}

Bomber::~Bomber()
{
	delete m_pBomberIdleSprite;
	m_pBomberIdleSprite = nullptr;

	delete m_pBomberAttack;
	m_pBomberAttack = nullptr;

	delete m_pBomberDeath;
	m_pBomberDeath = nullptr;
}

void Bomber::Draw() const
{
	DrawSprites();
}

void Bomber::Update(float elapsedSec, Level* level, Character* character, HUD* hud)
{
	UpdateSprites(elapsedSec);


	HandleHitAndDeath(elapsedSec);

	if (m_pBomberIdleSprite->IsPlayingBackwards())
	{
		if (m_pBomberIdleSprite->GetCurrentFrame() == 0)
		{
			if (!m_HasAttacked && m_BomberState != BomberState::death)
			{
				m_BomberState = BomberState::attack;
			}
		}
	}

	const Point2f Shootpos{ m_BomberShape.left + (m_BomberShape.width / 2.f),m_BomberShape.bottom + (m_BomberShape.height *0.3f) };
	if (m_pBomberAttack->GetCurrentFrame() == 4 && m_HasThrownBomb == false)
	{
		m_HasThrownBomb = true,
		m_pProjectileManger->AddProjectile(ProjectileManager::Type::bomb, Shootpos, m_pTextureManager, Vector2f{ 0,0 },m_pSoundManager);
	}

	if (m_HasAttacked && m_Timer <= m_MaxAttackCooldownTimer)
	{
		m_Timer += elapsedSec;
	}

	if (m_Timer > m_MaxAttackCooldownTimer)
	{
		m_HasAttacked = false;
		m_Timer = 0.f;
		m_MaxAttackCooldownTimer = m_OriginalMaximumAttackingTimer;
	}
}



const Rectf& Bomber::GetActorShape() const
{
	return m_BomberShape;
}

void Bomber::DrawSprites() const
{
	glPushMatrix();
	{
		glTranslatef(m_BomberShape.left, m_BomberShape.bottom,0);
		switch (m_BomberState)
		{
		case Bomber::BomberState::idle:
			m_pBomberIdleSprite->Draw();		
			break;
		case Bomber::BomberState::attack:
			m_pBomberAttack->Draw();
			break;
		case Bomber::BomberState::death:
			m_pBomberDeath->Draw();
			break;
		default:
			break;
		}
	}
	glPopMatrix();
}

void Bomber::UpdateSprites(float elapsedSec)
{
	switch (m_BomberState)
	{
	case Bomber::BomberState::idle:
		m_HasThrownBomb = false;
		m_pBomberAttack->SetCurrentFrame(0);
		if (!m_pBomberIdleSprite->IsPlayingBackwards())
		{
			if (!m_pBomberIdleSprite->IsOnLastFrame())
			{
				m_pBomberIdleSprite->Update(elapsedSec);
			}
			else
			{
				m_pBomberIdleSprite->SetPlayingBackwards(true);
				m_pBomberIdleSprite->PlayBackwards(elapsedSec);
				m_IsIdlePlayingBackwards = true;
			}
		}
		else
		{
			if (m_pBomberIdleSprite->GetCurrentFrame() == 0)
			{
				m_pBomberIdleSprite->SetPlayingBackwards(false);
				m_pBomberIdleSprite->SetCurrentFrame(0);
				m_IsIdlePlayingBackwards = false;
			}
			else
			{
				m_pBomberIdleSprite->PlayBackwards(elapsedSec);
			}
		}		break;
	case Bomber::BomberState::attack:
		if (!m_pBomberAttack->IsOnLastFrame())
		{
			m_pBomberAttack->Update(elapsedSec);
		}
		else
		{
			m_HasAttacked = true;
			if (m_IsIdlePlayingBackwards)
			{
				m_pBomberIdleSprite->SetPlayingBackwards(false);
				m_pBomberIdleSprite->SetCurrentFrame(m_pBomberIdleSprite->GetTotalFrames()-1);
				m_IsIdlePlayingBackwards = false;
			}
			m_BomberState = BomberState::idle;
			m_pBomberIdleSprite->HoldLastFrame();
		}
		break;
	case Bomber::BomberState::death:
		if (!m_pBomberDeath->IsOnLastFrame())
		{
			m_pBomberDeath->Update(elapsedSec);
		}
		break;
	}
}

void Bomber::HandleHitAndDeath(float elapsedSec)
{
	if (m_IsHit)
	{
		m_BomberState = BomberState::death;
	}

	if (m_BomberState == BomberState::death)
	{
		m_BomberShape.left += 200.f * elapsedSec;
		m_BomberShape.bottom -= 400.0f * elapsedSec;
	}


}


