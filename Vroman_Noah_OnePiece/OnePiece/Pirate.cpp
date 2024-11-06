#include "pch.h"
#include "Pirate.h"
#include <utils.h>
#include <iostream>
#include "Level.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Character.h"
#include "HUD.h"
#include "SoundManager.h"
#include <SoundEffect.h>
#include "ProjectileManager.h"
Pirate::Pirate(const Point2f& pos, TextureManager* textureManager, ProjectileManager* pProjectileManager,SoundManager* pSoundManager)
	:BaseEnemyClass{ textureManager,pSoundManager }
	,m_AttackDistance{ 180.0f }
	,m_DetectionDistance{ 300.f }
	,m_MoveSpeed{ 300.0f }
	,m_EnemyVelocity{ Vector2f{0,0} }
	,m_EnemyAcceleration{ Vector2f{0,-2000} }
	,m_RetreatDistance{ 100.0f }
	,m_HasAttacked{ false }
	,m_RotationAngle{ 0 }
	,m_IsDead{ false }
	,m_CannotFlip{ false }
	,m_Damaged{ false }
	,m_DamagedTimer{0.0f}
	,m_MaxDamageTimer{0.3f}
	,m_OriginalDamageTime{0.25f}
	,m_DeathTimer{ 0.0f }
	,m_OriginalDeathTimer{1.f}
	,m_MaxDeathTimer{1.f}
	

{
	m_EnemyShape.left = pos.x;
	m_EnemyShape.bottom = pos.y;

	m_AttackShape.width = 70;
	m_AttackShape.height = 50;

	m_pDeathSound = pSoundManager->GetSoundEffectPointer("DeathSound.wav");

	m_Damage = 1.f;
	m_Health = 4.f;
	m_OriginalHealth = m_Health;

	const std::string pirateIdle{ "PirateIdle.png" };
	const std::string pirateWalking{ "PirateWalking.png" };
	const std::string pirateKnifing{ "PirateKnifing.png" };
	const std::string PirateDying{ "PirateDying.png" };
	const std::string PirateDamaged{ "PirateDamaged.png" };
	const float scale{ 4 };

	m_pPirateIdle = new Sprite(pirateIdle, 3, 1, 6, Point2f{ 0,0 }, scale, textureManager);
	m_pPirateWalking = new Sprite(pirateWalking, 6, 1, 6, Point2f{ 0,0 }, scale, textureManager);
	m_pPirateKnifing = new Sprite(pirateKnifing, 5, 1, 5, Point2f{ 0,0 }, scale, textureManager);
	m_pPirateDying = new Sprite(PirateDying, 2, 1, 4, Point2f{ 0,0 }, scale, textureManager);
	m_pPirateDamaged = new Sprite(PirateDamaged, 1, 1, 1, Point2f{ 0,0 }, scale, textureManager);

	m_EnemyShape.width = m_pPirateIdle->GetFrameWidth() * scale;
	m_EnemyShape.height = m_pPirateIdle->GetFrameHeight() * scale;
}

Pirate::~Pirate()
{
	delete	m_pPirateIdle;
	m_pPirateIdle = nullptr;

	delete	m_pPirateWalking;
	m_pPirateWalking = nullptr;

	delete	m_pPirateKnifing;
	m_pPirateKnifing = nullptr;

	delete	m_pPirateDying;
	m_pPirateDying = nullptr;

	delete m_pPirateDamaged;
	m_pPirateDamaged = nullptr;
}

void Pirate::Draw() const
{
	DrawSprites();
}

void Pirate::Update(float elapsedSec, Level* level, Character* character, HUD* hud)
{


	UpdateSprites(elapsedSec);
	bool isEnemyOnGround = level->isOnGround(m_EnemyShape, m_EnemyVelocity);

	if (!isEnemyOnGround)
	{
		m_EnemyVelocity += m_EnemyAcceleration * elapsedSec;
		m_EnemyShape.bottom += m_EnemyVelocity.y * elapsedSec;

	}
	else
	{
		m_EnemyVelocity.y = 0;
	}

	if (!(m_ActionState == PirateActionState::attacking))
	{
		m_AttackShape.left = -500.f;
		m_AttackShape.bottom = -500.f;

	}
	if (m_ActionState == PirateActionState::idle || m_ActionState == PirateActionState::movingtowards || m_ActionState == PirateActionState::movingAway || m_ActionState == PirateActionState::attacking)
	{
		if (m_pPirateKnifing->GetCurrentFrame() == 3)
		{
			if (m_RotationAngle < 100)
			{
				m_AttackShape.left = m_EnemyShape.left - m_AttackShape.width;
				m_AttackShape.bottom = m_EnemyShape.bottom + m_EnemyShape.height * 0.3f;

			}
			if (m_RotationAngle > 100)
			{
				m_AttackShape.left = m_EnemyShape.left + m_EnemyShape.width;
				m_AttackShape.bottom = m_EnemyShape.bottom + m_EnemyShape.height * 0.3f;

			}

		}

	}
	if (m_ActionState != PirateActionState::dead)
	{
		if (m_IsHit)
		{
			m_Damaged = true;
			m_ActionState = PirateActionState::damaged;

		}

	}

	if (m_Health <= 0)
	{
		m_IsDead = true;

		m_pDeathSound->Play(0);
		m_ActionState = PirateActionState::dead;
	}

	if (m_Damaged == true && m_DamagedTimer <= m_MaxDamageTimer)
	{
		m_DamagedTimer += elapsedSec;
	}
	if (m_ActionState != PirateActionState::dead)
	{
		if (m_DamagedTimer > m_MaxDamageTimer)
		{
			m_Damaged = false;
			m_IsHit = false;
			m_ActionState = PirateActionState::movingtowards;
			m_DamagedTimer = 0.f;
			m_MaxDamageTimer = m_OriginalDamageTime;
		}
	}

	if (m_IsDead == true && m_DeathTimer <= m_MaxDeathTimer)
	{
		m_DeathTimer += elapsedSec;
	}
	if (m_DeathTimer > m_MaxDeathTimer)
	{
		m_IsDead = false;
		m_isFinished = true;
		m_DeathTimer = 0.f;
		m_MaxDeathTimer = m_OriginalDamageTime;
	}



	Point2f playerPosition{ character->GetPlayerShape().left,character->GetPlayerShape().height};
	m_CharacterPos = playerPosition;
	float distanceToPlayer = float(sqrt(pow(character->GetPlayerShape().left - m_EnemyShape.left, 2) + pow(character->GetPlayerShape().bottom - m_EnemyShape.bottom, 2)));

	LookAt(playerPosition);


	switch (m_ActionState)
	{
	case BaseEnemyClass::PirateActionState::idle:
		if (distanceToPlayer < m_DetectionDistance) {
			m_ActionState = BaseEnemyClass::PirateActionState::movingtowards;
		}
		break;

	case BaseEnemyClass::PirateActionState::movingtowards:
		if (distanceToPlayer < m_AttackDistance)
		{

			m_ActionState = BaseEnemyClass::PirateActionState::attacking;

		}
		else if (distanceToPlayer > m_RetreatDistance) {
			MoveTowards(playerPosition, elapsedSec);
		}
		else {
			LookAt(playerPosition);
			MoveForward(elapsedSec, playerPosition);
		}
		break;

	case BaseEnemyClass::PirateActionState::attacking:
		if (!m_HasAttacked) {
			m_HasAttacked = true;

			Attack();
		}
		if (m_pPirateKnifing->GetCurrentFrame() == 4)
		{
			m_ActionState = BaseEnemyClass::PirateActionState::movingAway;
		}

		break;

	case BaseEnemyClass::PirateActionState::movingAway:
		if (distanceToPlayer > m_DetectionDistance)
		{
			m_ActionState = BaseEnemyClass::PirateActionState::idle;
			ResetPosition();
		}
		else {
			MoveAway(playerPosition, elapsedSec);
			m_HasAttacked = false;
			m_pPirateKnifing->SetCurrentFrame(0);
		}
		break;

	}

	// Calculate distance to the player
	level->HandleCollison(m_EnemyShape, m_EnemyVelocity);
}

const Rectf& Pirate::GetActorShape() const
{
	return m_EnemyShape;
}

const Rectf& Pirate::GetAttackShape() const
{
	return m_AttackShape;
}

bool Pirate::GetLookingRight() const
{
	if (m_RotationAngle > 100)
	{
		return true;
	}
	return false;
}

float Pirate::GetHealth() const
{
	return m_Health;
}

void Pirate::DrawSprites() const
{
	glPushMatrix();
	{
		glTranslatef(m_EnemyShape.left, m_EnemyShape.bottom, 0);
		if (m_RotationAngle > 100 && !m_CannotFlip)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_EnemyShape.width, 0, 0);
		}
		switch (m_ActionState)
		{
		case BaseEnemyClass::PirateActionState::idle:
			m_pPirateIdle->Draw();
			break;
		case BaseEnemyClass::PirateActionState::movingtowards:
			m_pPirateWalking->Draw();
			break;
		case BaseEnemyClass::PirateActionState::movingAway:
			m_pPirateWalking->Draw();

			break;
		case BaseEnemyClass::PirateActionState::attacking:
			m_pPirateKnifing->SetDrawPos(Point2f{ -100.f,0.f });
			m_pPirateKnifing->Draw();
			break;
		case BaseEnemyClass::PirateActionState::dead:
			m_pPirateDying->Draw();
			break;

		case BaseEnemyClass::PirateActionState::damaged:
			m_pPirateDamaged->Draw();
			break;

		}

	}
	glPopMatrix();
}

void Pirate::UpdateSprites(float elapsedSec)
{

	switch (m_ActionState)
	{
	case BaseEnemyClass::PirateActionState::idle:
		m_CannotFlip = false;
		m_pPirateIdle->Update(elapsedSec);
		break;
	case BaseEnemyClass::PirateActionState::movingtowards:
		m_CannotFlip = false;

		m_pPirateWalking->Update(elapsedSec);
		break;
	case BaseEnemyClass::PirateActionState::movingAway:
		m_CannotFlip = false;

		m_pPirateWalking->Update(elapsedSec);

		break;
	case BaseEnemyClass::PirateActionState::attacking:
		m_CannotFlip = false;
		m_pPirateKnifing->Update(elapsedSec);
		break;
	case BaseEnemyClass::PirateActionState::dead:
		m_CannotFlip = true;

		if (!m_pPirateDying->IsOnLastFrame())
		{
			m_pPirateDying->Update(elapsedSec);
		}

		break;

	case BaseEnemyClass::PirateActionState::damaged:
		m_pPirateDamaged->Update(elapsedSec);
		break;

	}

}

void Pirate::Attack() {
	// TODO: Add attack logic here
}
void Pirate::ResetPosition()
{
	m_HasAttacked = false;

}

void Pirate::MoveTowards(const Point2f& targetPosition, float elapsedSec)
{
	float distanceToTarget = float(sqrt(pow(targetPosition.x - m_EnemyShape.left, 2) + pow(targetPosition.y - m_EnemyShape.bottom, 2)));
	float step = m_MoveSpeed * elapsedSec;

	if (distanceToTarget > step)
	{
		float distanceX = (targetPosition.x - m_EnemyShape.left) / distanceToTarget;
		float distanceY = (targetPosition.y - m_EnemyShape.bottom) / distanceToTarget;

		m_EnemyShape.left += distanceX * step;
	}
	else
	{
		m_EnemyShape.left = targetPosition.x;
	}
}

void Pirate::MoveAway(const Point2f& targetPosition, float elapsedSec)
{
	float distanceToTarget = float(sqrt(pow(targetPosition.x - m_EnemyShape.left, 2) + pow(targetPosition.y - m_EnemyShape.bottom, 2)));
	float step = m_MoveSpeed * elapsedSec;

	if (distanceToTarget > step) {
		float distanceX = (m_EnemyShape.left - targetPosition.x) / distanceToTarget;
		float distanceY = (m_EnemyShape.bottom - targetPosition.y) / distanceToTarget;
		m_EnemyShape.left += distanceX * step;
	}
	else
	{
		m_EnemyShape.left = targetPosition.x;

	}
}

void Pirate::LookAt(const Point2f& targetPosition)
{
	const float distanceX = targetPosition.x - m_EnemyShape.left;
	const float distanceY = targetPosition.y - m_EnemyShape.bottom;
	const float angle = atan2(distanceY, distanceX) * 180 / M_PI;
	m_RotationAngle = (angle < 0) ? 180 + angle : angle - 180;
}
void Pirate::MoveForward(float deltaTime, const Point2f& targetPosition)
{
	const float distanceX = targetPosition.x - m_EnemyShape.left;
	const float distanceY = targetPosition.y - m_EnemyShape.bottom;
	const float angle = atan2(distanceY, distanceX) * 180 / M_PI;

	if (m_HasAttacked) {
		float step = m_MoveSpeed * deltaTime;
		m_EnemyShape.left += step * float(cos(angle * M_PI / 180));
	}
}