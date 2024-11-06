#include "pch.h"
#include "BaseEnemyClass.h"
#include <utils.h>
#include <iostream>
#include "Character.h"
#include "HUD.h"
#include "SoundManager.h"
#include <SoundEffect.h>

BaseEnemyClass::BaseEnemyClass(TextureManager* textureManager, SoundManager* pSoundManager)
	:m_HorizontalSpeed{ 100.0f}
	,m_ActionState{PirateActionState::idle}
	, m_BossState{ BossState::spawn }
	,m_Health{10}
	,m_Damage{0}
	,m_AttackShape{Rectf{0,0,0,0}}
	,m_PointOfImpact{ 0 }
	,m_IsHit{ false }
	,m_isFinished{false}
	
{

	m_pHitEffectf = pSoundManager->GetSoundEffectPointer("Hit.wav");

}

BaseEnemyClass::~BaseEnemyClass()
{
}

void BaseEnemyClass::Update(float elapsedSec, Level* level, Character* character, HUD* hud)
{
}

void BaseEnemyClass::IsHit(const Rectf& characterAttackShape, float characterDamage)
{
	if (!m_IsHit)
	{
		if (utils::IsOverlapping(characterAttackShape, GetActorShape()))
		{
			m_IsHit = true;
			m_pHitEffectf->Play(0);
			m_Health -= characterDamage;
		}

	}
}


bool BaseEnemyClass::HitCharacter(const Rectf& characterShape)
{
	Point2f centerAttack{};

	//if (GetLookingRight())
	//{
	//	centerAttack.x = GetAttackShape().left + GetAttackShape().width / 2.f;
	//	centerAttack.y = GetAttackShape().bottom;

	//}
	//else
	//{
	//	centerAttack.x = GetAttackShape().left - GetAttackShape().width / 2.f;
	//	centerAttack.y = GetAttackShape().bottom;
	//}
	
	bool isPointInRect{};
	isPointInRect = utils::IsOverlapping(GetAttackShape(), characterShape);

	if (isPointInRect)
	{
		m_PointOfImpact = GetAttackShape().left;
		m_pHitEffectf->Play(0);
		return true;
	}

	return false;
}

bool BaseEnemyClass::IsFinished()const
{
	return m_isFinished;
}

float BaseEnemyClass::GetHealth() const
{
	return m_Health;
}


float BaseEnemyClass::GetPointOfInpact() const
{
	return m_PointOfImpact;
}

float BaseEnemyClass::GetDamage() const
{
	return m_Damage;
}



