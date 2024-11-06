#pragma once
#include "Vector2f.h"
class Level;
class TextureManager;
class Character;
class HUD;
class SoundManager;
class SoundEffect;

class BaseEnemyClass 
{
public:

	enum class PirateActionState
	{
		idle,
		movingtowards,
		movingAway,
		attacking,
		damaged,
		dead,
	};
	enum class BossState
	{
		spawn,
		throwingCape,
		idle,
		walking,
		attack1,
		attack2,
		damaged,
		death
	};
	BaseEnemyClass(TextureManager* pTextureManager,SoundManager* pSoundManager);
	virtual ~BaseEnemyClass();

	BaseEnemyClass(const BaseEnemyClass& other) = delete;
	BaseEnemyClass& operator=(const BaseEnemyClass& rhs) = delete;
	BaseEnemyClass(BaseEnemyClass&& other) = delete;
	BaseEnemyClass& operator=(BaseEnemyClass&& rhs) = delete;


	virtual void Draw() const =0;
	virtual void Update(float elapsedSec, Level* level, Character* character, HUD* hud);
	virtual const Rectf& GetAttackShape()const =0;

	void IsHit(const Rectf& characterAttackShape, float characterDamage);
	bool HitCharacter(const Rectf& characterShape);
	bool IsFinished()const;
    //virtual void Attack() = 0;

	virtual float GetHealth()const;

	virtual const Rectf& GetActorShape() const = 0;
	virtual bool GetLookingRight() const = 0;
	

	virtual float GetPointOfInpact() const;
	virtual float GetDamage()const;

protected:
	
	///////////////////
	/// ENEMY VARIABLES
	///////////////////
	float				 m_Health;
	float				 m_Damage;
	const float			 m_HorizontalSpeed;
	float				 m_PointOfImpact;
	
	bool			     m_IsHit;
	bool			     m_isFinished;

	SoundEffect* m_pHitEffectf;

	Rectf				 m_AttackShape;

	PirateActionState m_ActionState;
	BossState m_BossState;


};

