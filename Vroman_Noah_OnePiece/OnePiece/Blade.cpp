#include "pch.h"
#include "Blade.h"
#include "TextureManager.h"
#include "Sprite.h"
#include <utils.h>
#include <iostream>
#include "Level.h"
Blade::Blade(const Point2f& position,TextureManager* pTextureManager, const Vector2f& velocity)
	:Projectile(position,pTextureManager,velocity)
	,m_OtherVelocity{velocity}
{
	m_MoveSpeed = 1000.0f;
	m_ProjectileShape.left = position.x;
	m_ProjectileShape.bottom = position.y;
	m_ProjectileShape.width = 140.0f;
	m_ProjectileShape.height = 76.f;

	m_Damage = 1;

	m_pBladeProjectile = new Sprite("Projectile.png", 1, 1, 0, Point2f{ 0,0 }, 4.f, pTextureManager);
}


Blade::~Blade()
{
	delete m_pBladeProjectile;
	m_pBladeProjectile = nullptr;
}

void Blade::Draw() const
{


	glPushMatrix();
	{
		glTranslatef(m_ProjectileShape.left, m_ProjectileShape.bottom,0);

		if (m_OtherVelocity.x > 0)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_ProjectileShape.width, 0, 0);
		}

		m_pBladeProjectile->Draw();
	}
	glPopMatrix();
}

void Blade::Update(float elapsedSec, Level* level,const Rectf& characterShape)
{
	if (m_OtherVelocity.x >= 0)
	{
		m_ProjectileVelocity.x = m_MoveSpeed;
	}
	else
	{
		m_ProjectileVelocity.x = -m_MoveSpeed;
	}


	m_ProjectileShape.left += m_ProjectileVelocity.x * elapsedSec;


	if (m_ProjectileShape.left < 0)
	{
		m_isFinished = true;
	}
}
