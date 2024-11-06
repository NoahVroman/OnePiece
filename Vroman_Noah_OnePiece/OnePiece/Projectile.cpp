#include "pch.h"
#include "Projectile.h"
#include <utils.h>
#include "TextureManager.h"
#include "Level.h"
Projectile::Projectile(const Point2f& position,TextureManager* pTextureManager, const Vector2f& velocity)
	:m_Damage{0}
	,m_ProjectileShape{Rectf{0,0,0,0}}
	,m_ProjectileVelocity{Vector2f{0,0}}
	,m_ProjectileAcceleration{Vector2f{0,0}}
	,m_MoveSpeed{ 0 }
	,m_isFinished{false}
{
	m_ProjectileShape.left = 0;
	m_ProjectileShape.bottom = 0;
	m_ProjectileShape.width = 0;
	m_ProjectileShape.height = 0;


}

Projectile::~Projectile()
{
}

bool Projectile::IsOverlapping(const Rectf& character)
{
	return utils::IsOverlapping(m_ProjectileShape,character);
}

const Rectf& Projectile::GetShape()
{
	return m_ProjectileShape;
}

int Projectile::GetDamage() const
{
	return m_Damage;
}

bool Projectile::GetIsFinished() const
{
	return m_isFinished;
}
