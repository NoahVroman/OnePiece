#include "pch.h"
#include "Bomb.h"
#include "TextureManager.h"
#include "Sprite.h"
#include <utils.h>
#include "Level.h"
#include <iostream>
#include "SoundManager.h"
#include <SoundEffect.h>


Bomb::Bomb(const Point2f& position, TextureManager* pTextureManager, const Vector2f& velocity, SoundManager* pSoundManager)
	:Projectile(position, pTextureManager, velocity)
	,m_isArmed{false}
	,m_BombTimer{0.f}
	,m_MaxBombCooldownTimer{3.f}
	,m_OriginalMaximumArmedTimer{ 3.f }
	,m_BombState{BombState::bomb}
	,m_NumberOfBounces{ 0 }
	,m_HasHit{false}
	
{
	float scale{ 4 };
	m_BombShape.left = position.x;
	m_BombShape.bottom = position.y;
	m_BombShape.width = 36.f;
	m_BombShape.height = 64.f;

	m_pExplosionEffect = pSoundManager->GetSoundEffectPointer("BombExplosion.wav");
	m_pBombEffect = pSoundManager->GetSoundEffectPointer("Bomb.wav");


	m_pBombEffect->Play(0);

	m_Damage = 3;

	m_ProjectileShape.width = m_BombShape.width * 5.f;
	m_ProjectileShape.height = m_BombShape.height * 2.f;

	m_ProjectileVelocity = Vector2f{ 0,0 };
	m_ProjectileAcceleration = Vector2f{ 0,-981.f };

	m_pBomb = new Sprite("Bomb.png", 2, 1, 4, Point2f{ 0,0 }, scale, pTextureManager);
	m_pBombArmed = new Sprite("BombArmed.png", 2, 1, 4, Point2f{ 0,0 }, scale, pTextureManager);
	m_pBombExplosion = new Sprite("BombExplosions.png", 6, 1, 6, Point2f{ 0,0 }, scale, pTextureManager);

}

Bomb::~Bomb()
{
	delete m_pBomb;
	m_pBomb = nullptr;

	delete m_pBombArmed;
	m_pBombArmed = nullptr;

	delete m_pBombExplosion;
	m_pBombExplosion = nullptr;
}

void Bomb::Draw() const
{
	DrawSprites();
}

void Bomb::Update(float elapsedSec,Level* level,const Rectf& characterShape)
{
	UpdateSprites(elapsedSec);

	if (utils::IsOverlapping(characterShape, m_BombShape) && !m_HasHit)
	{
		m_HasHit = true;
		m_pExplosionEffect->Play(0);

		m_BombState = BombState::explosion;
	}

	if (!m_HasHit)
	{
		if (m_NumberOfBounces <= 1)
		{
			if (!level->isOnGround(m_BombShape, m_ProjectileVelocity))
			{
				m_ProjectileVelocity += m_ProjectileAcceleration * elapsedSec;
				m_BombShape.bottom += m_ProjectileVelocity.y * elapsedSec;
				m_BombShape.left += m_ProjectileVelocity.x * elapsedSec;
				level->HandleCollison(m_BombShape, m_ProjectileVelocity);

			}
			else
			{
				++m_NumberOfBounces;

				m_ProjectileVelocity.y = -m_ProjectileAcceleration.y;

				m_ProjectileVelocity *= 0.4f;

				m_BombShape.bottom += m_ProjectileVelocity.y * elapsedSec;
			}
		}
		if (m_NumberOfBounces == 1)
		{
			m_isArmed = true;
			m_BombState = BombState::armed;
		}

	}





	if (m_BombState == BombState::armed)
	{
		if (m_isArmed && m_BombTimer <= m_MaxBombCooldownTimer)
		{
			m_BombTimer += elapsedSec;
		}

		if (m_BombTimer >= 1.f)
		{
			m_pBombArmed->SetFramerate(8);
		}
		if (m_BombTimer >= 2.f)
		{
			m_pBombArmed->SetFramerate(16);
		}

		if (m_BombTimer > m_MaxBombCooldownTimer)
		{
			m_isArmed = false;
			m_pExplosionEffect->Play(0);

			m_BombState = BombState::explosion;
			m_BombTimer = 0.f;
			m_MaxBombCooldownTimer = m_OriginalMaximumArmedTimer;
		}

	}


}

void Bomb::DrawSprites() const
{

	glPushMatrix();
	{
		glTranslatef(m_BombShape.left, m_BombShape.bottom,0);
		switch (m_BombState)
		{
		case Bomb::BombState::bomb:
			m_pBomb->Draw();
			break;
		case Bomb::BombState::armed:
			m_pBombArmed->Draw();
			break;
		case Bomb::BombState::explosion:
			m_pBombExplosion->SetDrawPos(Point2f{ -35.f,0 });
			m_pBombExplosion->Draw();
			break;
		}
	}
	glPopMatrix();
}

void Bomb::UpdateSprites(float elapsedSec)
{
	switch (m_BombState)
	{
	case Bomb::BombState::bomb:
		m_pBomb->Update(elapsedSec);

		break;
	case Bomb::BombState::armed:
		m_pBombArmed->Update(elapsedSec);
		break;
	case Bomb::BombState::explosion:

		if (m_pBombExplosion->GetCurrentFrame() <= 0)
		{
			m_ProjectileShape.left = (m_BombShape.left + m_BombShape.width / 2.f) - m_ProjectileShape.width / 2.f;
			m_ProjectileShape.bottom = m_BombShape.bottom - m_BombShape.height / 2.f;

		}
		else
		{
			m_ProjectileShape.left =0;
			m_ProjectileShape.bottom = 0;
		}

		if (!m_pBombExplosion->IsOnLastFrame())
		{
			m_pBombExplosion->Update(elapsedSec);
		}
		else
		{
			m_isFinished = true;

		}
		break;
	}
}
