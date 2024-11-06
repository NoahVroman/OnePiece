#pragma once
#include "BaseEnemyClass.h"
class Level;
class Sprite;
class TextureManager;
class Character;
class HUD;
class ProjectileManager;
class SoundManager;
class SoundEffect;

class Pirate final : public BaseEnemyClass
{
public:
	Pirate(const Point2f& pos, TextureManager* textureManager, ProjectileManager* pProjectileManager,SoundManager* pSoundManager);
	~Pirate();

	Pirate(const Pirate& other) = delete;
	Pirate& operator=(const Pirate& rhs) = delete;
	Pirate(Pirate&& other) = delete;
	Pirate& operator=(Pirate&& rhs) = delete;


	virtual void Draw() const override;
	virtual void Update(float elapsedSec, Level* level, Character* character, HUD* hud) override;
	virtual const Rectf& GetActorShape() const override;
	virtual const Rectf& GetAttackShape()const override;
	virtual bool GetLookingRight() const override;
	virtual float GetHealth()const override;



private:

	void DrawSprites() const;
	void UpdateSprites(float elapsedSec) ;
	void ResetPosition();
	void MoveTowards(const Point2f& targetPosition, float elapsedSec);
	void MoveAway(const Point2f& targetPosition, float elapsedSec);
	void LookAt(const Point2f& targetPosition);
	void MoveForward(float deltaTime, const Point2f& targetPosition);

	void Attack();


	SoundEffect* m_pDeathSound;

	Rectf m_EnemyShape;
	Vector2f m_EnemyVelocity;
	Vector2f m_EnemyAcceleration;


	Sprite* m_pPirateIdle;
	Sprite* m_pPirateWalking;
	Sprite* m_pPirateKnifing;
	Sprite* m_pPirateDying;
	Sprite* m_pPirateDamaged;

	float m_OriginalHealth;

	const float m_MoveSpeed;
	const float m_DetectionDistance;
	const float m_AttackDistance;
	const float m_RetreatDistance;

	Point2f m_CharacterPos;
	
	bool m_IsDead;
	bool m_CannotFlip;
	float m_RotationAngle;
	bool m_HasAttacked;

	////////////////
	//   Timers   //
	////////////////

	float m_DeathTimer;
	float m_MaxDeathTimer;
	const float m_OriginalDeathTimer;
	

	float m_DamagedTimer;
	float m_MaxDamageTimer;
	bool m_Damaged;
	const float m_OriginalDamageTime;



};

