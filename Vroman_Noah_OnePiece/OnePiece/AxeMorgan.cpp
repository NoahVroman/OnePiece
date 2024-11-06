#include "pch.h"
#include "AxeMorgan.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Level.h"
#include <utils.h>
#include <iostream>
#include "Character.h"
#include "HUD.h"
#include "SoundManager.h"
#include <SoundEffect.h>
#include"ProjectileManager.h"
AxeMorgan::AxeMorgan(const Point2f& pos, TextureManager* pTextureManager, ProjectileManager* pProjectileManager, SoundManager* pSoundManager)
	:BaseEnemyClass(pTextureManager,pSoundManager)
	,m_AxeMorganAcceleration{ Vector2f{0,-2000} }
	,m_AxeMorganVelocity{Vector2f{0,0}}
	,m_MoveSpeed{100.0f}
	,m_CapeSpeed{300.0f}
	,m_CapeAcceleration{ Vector2f{ 0, 0} }
	,m_CapeVelocity{ Vector2f{0,0} }
	,m_IsSetToIdle{false}
	,m_IsSetToPrevious{ false }
	,m_Timer{ 0 }
	,m_MaxAttackCooldownTimer{3.f}
	,m_OriginalMaximumAttackingTimer{3.f}
	,m_HasAttacked{false}
	,m_SpawnBulletOnce{false}
	,m_Damaged{ false }
	,m_DamagedTimer{ 0.0f }
	,m_MaxDamageTimer{ 0.25f }
	,m_OriginalDamageTime{ 0.25f }
	,m_DistanceToTarget{0.f}
	,m_DistanceFromBoss{400.0f}

{
	const float scale{ 4.f };
	m_pTextureManager = pTextureManager;
	m_pProjectileManager = pProjectileManager;
	m_pSoundManager = pSoundManager;
	m_Health = 10.0f;
	m_Damage = 1.f;

	m_AxeMorganHitbox.left = pos.x;
	m_AxeMorganHitbox.bottom = pos.y;


	m_pAxeMorganSpawn	= new Sprite("AxeMorganStart.png",4,1,8,Point2f{0,0},scale,pTextureManager);
	m_pAxeMorganWalking	= new Sprite("AxeMorganWalking.png",5,1,7,Point2f{0,0},scale,pTextureManager);
	m_pAxeMorganAttack1	= new Sprite("AxeMorganAttack.png",5,1,6,Point2f{0,0},scale,pTextureManager);
	m_pAxeMorganAttack2 = new Sprite("AxeMorganAttack2.png", 6, 1,10, Point2f{ 0,0 }, scale, pTextureManager);
	m_AxeMorganThrowingCape = new Sprite("AxeMorganThrowingCape.png", 6, 1, 6, Point2f{ 0,0 }, scale, pTextureManager);
	m_pAxeMorganImpact	= new Sprite("AxeMorganImpact.png",3,1,10,Point2f{0,0},scale,pTextureManager);
	m_pAxeMorganDamaged	= new Sprite("AxeMorganDamaged.png",3,1,6,Point2f{0,0},scale,pTextureManager);
	m_pAxeMorganDeath	= new Sprite("AxeMorganDeath.png",4,1,6,Point2f{0,0},scale,pTextureManager);
	m_pCape = new Sprite("Cape.png", 2, 1, 6, Point2f{ 0,0 }, scale, pTextureManager);
	
	m_AxeMorganHitbox.width = 200.0f;
	m_AxeMorganHitbox.height = 264.f;


	m_pBossAttackEffect = pSoundManager->GetSoundEffectPointer("BossAttack.wav");

	m_Cape.left = (m_AxeMorganHitbox.left + (m_AxeMorganHitbox.width / 2.f));
	m_Cape.bottom = (m_AxeMorganHitbox.bottom + (m_AxeMorganHitbox.height * 0.3f));

}

AxeMorgan::~AxeMorgan()
{
		 delete m_pAxeMorganSpawn;
		 m_pAxeMorganSpawn = nullptr;

		 delete m_pAxeMorganWalking;
		 m_pAxeMorganWalking =nullptr;

		 delete m_pAxeMorganAttack1;
		 m_pAxeMorganAttack1 =nullptr;

		 delete m_pAxeMorganAttack1;
		 m_pAxeMorganAttack1 = nullptr;

		 delete m_pAxeMorganAttack2;
		 m_pAxeMorganAttack2 = nullptr;

		 delete m_pAxeMorganImpact;
		 m_pAxeMorganImpact =nullptr;

		 delete m_pAxeMorganDamaged;
		 m_pAxeMorganDamaged = nullptr;

		 delete m_pAxeMorganDeath;
		 m_pAxeMorganDeath = nullptr;
		 
		 delete m_AxeMorganThrowingCape;
		 m_AxeMorganThrowingCape = nullptr;

		 delete m_pCape;
		 m_pCape = nullptr;


}

void AxeMorgan::Draw() const
{
	DrawSprites();
}

void AxeMorgan::Update(float elapsedSec, Level* level, Character* character,HUD* hud)
{

	m_DistanceToTarget = utils::GetDistance(Point2f{ m_AxeMorganHitbox.left,m_AxeMorganHitbox.bottom }, Point2f{ character->GetPlayerShape().left,character->GetPlayerShape().bottom });
	hud->UpdateBoss(elapsedSec, this);
	UpdateSprites(elapsedSec);

	const bool onGround = level->isOnGround(m_AxeMorganHitbox, m_AxeMorganVelocity);
	if (!onGround)
	{
		m_AxeMorganVelocity += m_AxeMorganAcceleration * elapsedSec;
		m_AxeMorganHitbox.bottom += m_AxeMorganVelocity.y * elapsedSec;
	}
	else
	{
		m_AxeMorganVelocity.y = 0;
	}

	HandleSpawn(character, elapsedSec);
	HandleWalking(character, elapsedSec);
	HandleAttack();
	HandleDeath();
	HandleTimers(elapsedSec);
	HandleShooting();



}

const Rectf& AxeMorgan::GetAttackShape() const
{
	return m_ShapeAttack;
}

const Rectf& AxeMorgan::GetActorShape() const
{
	return m_AxeMorganHitbox;
}

float AxeMorgan::GetHealth() const
{
	return m_Health;
}

void AxeMorgan::DrawSprites() const
{
	glPushMatrix();
	{
		glTranslatef(m_AxeMorganHitbox.left, m_AxeMorganHitbox.bottom, 0);
		if (m_AxeMorganVelocity.x > 0)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_AxeMorganHitbox.width, 0, 0);
		}
		switch (m_BossState)
		{
		case AxeMorgan::BossState::spawn:
			m_pAxeMorganSpawn->Draw();
			break;
		case AxeMorgan::BossState::throwingCape:
			m_AxeMorganThrowingCape->Draw();
			break;
		case AxeMorgan::BossState::idle:
			break;
		case AxeMorgan::BossState::walking:
			m_pAxeMorganWalking->Draw();
			break;
		case AxeMorgan::BossState::attack1:
			m_pAxeMorganAttack1->SetDrawPos(Point2f{ -120,0 });
			m_pAxeMorganAttack1->Draw();
			break;
		case AxeMorgan::BossState::attack2:
			m_pAxeMorganAttack2->SetDrawPos(Point2f{ -200.0f,0 });
			m_pAxeMorganAttack2->Draw();
			break;
		case AxeMorgan::BossState::damaged:
			m_pAxeMorganDamaged->Draw();
			break;
		case AxeMorgan::BossState::death:
			m_pAxeMorganDeath->Draw();
			break;
		default:
			break;
		}
	}
	glPopMatrix();

	glPushMatrix(); 
	{
		glTranslatef(m_Cape.left, m_Cape.bottom, 0);

		if ((m_Cape.bottom + m_Cape.height) > -400) 
		{
			if (m_AxeMorganThrowingCape->GetCurrentFrame() >= 2)
			{

				m_pCape->Draw();
			}

		}

	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(m_ShapeAttack.left, m_ShapeAttack.bottom, 0);
		if (m_pAxeMorganAttack1->GetCurrentFrame() >= 2 && m_BossState == BossState::attack1)
		{
			if (m_AxeMorganVelocity.x > 0)
			{
				glScalef(-1, 1, 1);
				glTranslatef(-m_ShapeAttack.width, 0, 0);
			}
			m_pAxeMorganImpact->Draw();


		}
	}
	glPopMatrix();


}

void AxeMorgan::UpdateSprites(float elapsedSec)
{
	switch (m_BossState)
	{
	case AxeMorgan::BossState::spawn:
		m_pAxeMorganSpawn->Update(elapsedSec);
		break;
	case AxeMorgan::BossState::throwingCape:
		m_AxeMorganThrowingCape->Update(elapsedSec);
		if (m_AxeMorganThrowingCape->IsOnLastFrame())
		{
			m_BossState = BossState::walking;
		}
		break;
	case AxeMorgan::BossState::idle:

		break;
	case AxeMorgan::BossState::walking:
		m_SpawnBulletOnce = false;
		m_pAxeMorganImpact->SetCurrentFrame(0);
		m_pAxeMorganAttack1->SetCurrentFrame(0);
		m_pAxeMorganDamaged->SetCurrentFrame(0);
		m_pAxeMorganAttack2->SetCurrentFrame(0);

		m_pAxeMorganWalking->Update(elapsedSec);

		break;
	case AxeMorgan::BossState::attack1:
		m_pAxeMorganDamaged->SetCurrentFrame(0);

		m_pAxeMorganAttack1->Update(elapsedSec);
		if (m_pAxeMorganAttack1->GetCurrentFrame() >= 2)
		{
			m_pAxeMorganImpact->Update(elapsedSec);
		}
		if (m_pAxeMorganAttack1->IsOnLastFrame())
		{
			m_BossState = BossState::walking;
		}
		break;
	case AxeMorgan::BossState::attack2:
		m_pAxeMorganDamaged->SetCurrentFrame(0);

		m_pAxeMorganAttack2->Update(elapsedSec);

		if (m_pAxeMorganAttack2->IsOnLastFrame())
		{
			m_HasAttacked = true;
			m_BossState = BossState::walking;
		}

		break;
	case AxeMorgan::BossState::damaged:
		if (!m_pAxeMorganDamaged->IsOnLastFrame())
		{
			m_pAxeMorganDamaged->Update(elapsedSec);
		}
		break;
	case AxeMorgan::BossState::death:
		if (!m_pAxeMorganDeath->IsOnLastFrame())
		{
			m_pAxeMorganDeath->Update(elapsedSec);
		}
		break;
	default:
		break;
	}
	
	if (m_AxeMorganThrowingCape->GetCurrentFrame() >= 2)
	{
		if (!m_pCape->IsOnLastFrame())
		{
			m_pCape->Update(elapsedSec);
		}
	}
}

void AxeMorgan::HandleSpawn(Character* character,float elapsedSec)
{
	Character::CharacterActionState prevActionState = character->GetSate();


	if (m_DistanceToTarget <= m_DistanceFromBoss && m_BossState == BossState::spawn)
	{
		m_BossState = BossState::throwingCape;
		if (!m_IsSetToIdle)
		{
			character->SetEnableInput(false);
			m_IsSetToIdle = true;
			character->SetActionSate(Character::CharacterActionState::idle);

		}
	}

	if (m_AxeMorganThrowingCape->IsOnLastFrame())
	{

		if (!m_IsSetToPrevious && m_IsSetToIdle)
		{
			character->SetEnableInput(true);
			character->SetActionSate(prevActionState);
			m_IsSetToPrevious = true;
		}

	}


	if (m_Cape.left <= 2000)
	{
		if (m_AxeMorganThrowingCape->GetCurrentFrame() >= 2)
		{

			m_CapeVelocity.x = m_CapeSpeed;
			m_CapeVelocity.y = m_CapeAcceleration.y;

			m_Cape.left += m_CapeVelocity.x * elapsedSec;
			m_Cape.bottom += m_AxeMorganVelocity.y * elapsedSec;
		}
	}

}

void AxeMorgan::HandleWalking(Character* character,float elapsedSec)
{
	if (m_BossState == BossState::walking)
	{
		if (m_AxeMorganHitbox.left >= character->GetPlayerShape().left + character->GetPlayerShape().width)
		{
			m_AxeMorganVelocity.x = -m_MoveSpeed;
		}
		else if (m_AxeMorganHitbox.left + m_AxeMorganHitbox.width <= character->GetPlayerShape().left)
		{
			m_AxeMorganVelocity.x = m_MoveSpeed;
		}
		else
		{
			m_AxeMorganVelocity.x = 0;
		}
		m_AxeMorganHitbox.left += m_AxeMorganVelocity.x * elapsedSec;

	}


}

void AxeMorgan::HandleAttack()
{
	if ((m_pAxeMorganAttack1->GetCurrentFrame() < 2) || (m_BossState != BossState::attack1))
	{
		m_ShapeAttack.left = -500.0f;
		m_ShapeAttack.bottom = -500.0f;
	}
	else if (m_AxeMorganVelocity.x > 0)
	{
		m_pBossAttackEffect->Play(0);
		m_ShapeAttack.width = 180;
		m_ShapeAttack.height = 150;
		m_ShapeAttack.left = m_AxeMorganHitbox.left + m_AxeMorganHitbox.width * 0.8f;
		m_ShapeAttack.bottom = m_AxeMorganHitbox.bottom - (m_ShapeAttack.height * 0.25f);
	}
	else if (m_AxeMorganVelocity.x < 0)
	{
		m_pBossAttackEffect->Play(0);

		m_ShapeAttack.width = 180;
		m_ShapeAttack.height = 150;
		m_ShapeAttack.left = m_AxeMorganHitbox.left - m_ShapeAttack.width * 0.8f;
		m_ShapeAttack.bottom = m_AxeMorganHitbox.bottom - (m_ShapeAttack.height * 0.25f);
	}

	if (m_Health <= 5.f)
	{
		if (m_DistanceToTarget >= 500.0f)
		{
			if (!m_HasAttacked)
			{
				m_BossState = BossState::attack2;
			}
		}
	}
	if (m_DistanceToTarget <= 300.0f)
	{
		m_BossState = BossState::attack1;
	}

}

void AxeMorgan::HandleDeath()
{
	if (!(m_BossState == BossState::death))
	{
		if (m_IsHit)
		{
			m_Damaged = true;
			m_BossState = BossState::damaged;
		}

	}

	if (m_Health <= 0)
	{
		m_BossState = BossState::death;
	}

}

void AxeMorgan::HandleTimers(float elapsedSec)
{
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


	if (!(m_BossState == BossState::death))
	{

		if (m_Damaged == true && m_DamagedTimer <= m_MaxDamageTimer)
		{
			m_DamagedTimer += elapsedSec;
		}
		if (m_DamagedTimer > m_MaxDamageTimer)
		{
			m_Damaged = false;
			m_IsHit = false;
			m_BossState = BossState::walking;
			m_DamagedTimer = 0.f;
			m_MaxDamageTimer = m_OriginalDamageTime;
		}
	}



}

void AxeMorgan::HandleShooting()
{
	Point2f ShootPos{};

	if (m_AxeMorganVelocity.x > 0)
	{
		ShootPos = Point2f{ m_AxeMorganHitbox.left + m_AxeMorganHitbox.width, m_AxeMorganHitbox.bottom + (m_AxeMorganHitbox.height * 0.35f) };
	}
	else
	{
		ShootPos = Point2f{ m_AxeMorganHitbox.left - m_pProjectileManager->GetProjectileShape().width, m_AxeMorganHitbox.bottom + (m_AxeMorganHitbox.height * 0.35f) };

	}

	if (m_pAxeMorganAttack2->GetCurrentFrame() == 3 && m_SpawnBulletOnce == false)
	{
		m_SpawnBulletOnce = true;
		m_pProjectileManager->AddProjectile(ProjectileManager::Type::blade, ShootPos, m_pTextureManager, m_AxeMorganVelocity, m_pSoundManager);
	}

}
