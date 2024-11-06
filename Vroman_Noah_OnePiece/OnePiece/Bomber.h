#pragma once
#include "BaseEnemyClass.h"
class TextureManager;
class ProjectileManager;
class Sprite;
class SoundManager;
class SoundEffect;

class Bomber final : public BaseEnemyClass
{
public:
	
	Bomber(const Point2f& pos, TextureManager* pTextureManager, ProjectileManager* pProjectileManager,SoundManager* pSoundManager);
	~Bomber();

	enum class BomberState 
	{
		idle,
		attack,
		death
	};

	Bomber(const Bomber& other) = delete;
	Bomber& operator=(const Bomber& rhs) = delete;
	Bomber(Bomber&& other) = delete;
	Bomber& operator=(Bomber&& rhs) = delete;



	virtual void Draw() const override;
	virtual void Update(float elapsedSec, Level* level, Character* character, HUD* hud) override;
	virtual const Rectf& GetAttackShape()const override { return Rectf{ 0,0,0,0 }; };
	virtual const Rectf& GetActorShape() const override;
	virtual bool GetLookingRight() const override { return false; };

private:

	void DrawSprites() const;
	void UpdateSprites(float elapsedSec);
	void HandleHitAndDeath(float elapsedSec);


	BomberState m_BomberState;

	Rectf m_BomberShape;

	Sprite* m_pBomberIdleSprite;
	Sprite* m_pBomberAttack;
	Sprite* m_pBomberDeath;

	bool m_IsIdlePlayingBackwards;

	ProjectileManager* m_pProjectileManger;
	TextureManager* m_pTextureManager;

	bool m_HasThrownBomb;

	bool  m_HasAttacked;
	float m_Timer;
	const float m_OriginalMaximumAttackingTimer;
	float m_MaxAttackCooldownTimer;

	SoundManager* m_pSoundManager;

};

