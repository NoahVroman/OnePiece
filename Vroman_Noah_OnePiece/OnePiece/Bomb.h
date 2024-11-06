#pragma once
#include "Projectile.h"
#include <Vector2f.h>
#include "TextureManager.h"
#include "Sprite.h"
class SoundManager;
class SoundEffect;
class Bomb final : public Projectile
{
public:
	Bomb(const Point2f& position, TextureManager* pTextureManager, const Vector2f& velocity,SoundManager* pSoundManager);
	~Bomb();

	Bomb(const Bomb& other) = delete;
	Bomb& operator=(const Bomb& rhs) = delete;
	Bomb(Bomb&& other) = delete;
	Bomb& operator=(Bomb&& rhs) = delete;

	enum class BombState
	{
		bomb,
		armed,
		explosion
	};


	virtual void Draw()const override;
	virtual void Update(float elapsedSec, Level* level, const Rectf& characterShape) override;


private:

	void DrawSprites() const;
	void UpdateSprites(float elapsedSec);

	SoundEffect* m_pExplosionEffect;
	SoundEffect* m_pBombEffect;

	Rectf m_BombShape;
	Rectf m_ExplosionShape;

	Sprite* m_pBomb;
	Sprite* m_pBombArmed;
	Sprite* m_pBombExplosion;

	BombState m_BombState;

	int m_NumberOfBounces;

	bool m_HasHit;

	bool  m_isArmed;
	float m_BombTimer;
	const float m_OriginalMaximumArmedTimer;
	float m_MaxBombCooldownTimer;

};

