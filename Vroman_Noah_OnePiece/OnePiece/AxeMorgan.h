#pragma once
#include "BaseEnemyClass.h"
#include <Vector2f.h>
class TextureManager;
class Sprite;
class Level;
class Character;
class HUD;
class ProjectileManager;
class SoundManager;
class SoundEffect;

class AxeMorgan final : public BaseEnemyClass
{
public:

	AxeMorgan(const Point2f& pos, TextureManager* pTextureManager,ProjectileManager* pProjectileManager,SoundManager* pSoundManager);
	~AxeMorgan();


    AxeMorgan(const Point2f& pos ,const AxeMorgan& other) = delete;
    AxeMorgan& operator=(const AxeMorgan& rhs) = delete;
    AxeMorgan(AxeMorgan&& other) = delete;
    AxeMorgan& operator=(AxeMorgan&& rhs) = delete;

    virtual void Draw() const override;
    virtual void Update(float elapsedSec, Level* level, Character* character, HUD* hud) override;
    virtual const Rectf& GetAttackShape()const override;
	virtual const Rectf& GetActorShape() const override;
	virtual bool GetLookingRight() const override { return false; };
	virtual float GetHealth()const override;



private:

	void DrawSprites() const;
	void UpdateSprites(float elapsedSec);
	void HandleSpawn(Character* character,float elapsedSec);
	void HandleWalking(Character* character, float elapsedSec);
	void HandleAttack();
	void HandleDeath();
	void HandleTimers(float elapsedSec);
	void HandleShooting();
	//////////////////
	//	  Sprite   //
	/////////////////

	Sprite* m_pAxeMorganSpawn	;
	Sprite* m_pAxeMorganWalking	;
	Sprite* m_pAxeMorganAttack1	;
	Sprite* m_pAxeMorganAttack2;
	Sprite* m_AxeMorganThrowingCape;
	Sprite* m_pAxeMorganImpact	;
	Sprite* m_pAxeMorganDamaged	;
	Sprite* m_pAxeMorganDeath	;
	Sprite* m_pCape;


	/////////////////////////////
	//	  Shape and Movement   //
	////////////////////////////

	const float m_MoveSpeed;
	
	Rectf	 m_AxeMorganHitbox;
	Vector2f m_AxeMorganVelocity;
	const Vector2f m_AxeMorganAcceleration;


	//////////////
	//	Cape   //
	/////////////

	const float m_CapeSpeed;
	Rectf m_Cape;
	Vector2f m_CapeVelocity;
	const Vector2f m_CapeAcceleration;

	 ////////////////
	//	Attack    //
   ////////////////

	Rectf m_ShapeAttack;

	//////////////
	//	Bool   //
	/////////////

	bool m_IsSetToIdle;
	bool m_IsSetToPrevious;
	bool m_SpawnBulletOnce;

	/////////////////
	//	  Timers   //
	/////////////////

	bool  m_HasAttacked;
	float m_Timer;
	const float m_OriginalMaximumAttackingTimer;
    float m_MaxAttackCooldownTimer;


	float m_DamagedTimer;
	float m_MaxDamageTimer;
	bool m_Damaged;
	const float m_OriginalDamageTime;


	SoundEffect* m_pBossAttackEffect;

	TextureManager* m_pTextureManager;
	ProjectileManager* m_pProjectileManager;
	SoundManager* m_pSoundManager;

	float m_DistanceToTarget;
	float m_DistanceFromBoss;
};

