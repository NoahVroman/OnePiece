#pragma once
#include <Vector2f.h>
#include <Vector2f.h>
class TextureManager;
class Level;
class Projectile
{
public:
	Projectile(const Point2f& position,TextureManager* pTextureManager,const Vector2f& velocity);
	virtual ~Projectile();

	Projectile(const Projectile& other) = delete;
	Projectile& operator=(const Projectile& rhs) = delete;
	Projectile(Projectile&& other) = delete;
	Projectile& operator=(Projectile&& rhs) = delete;


	virtual void Draw()const = 0;
	virtual void Update(float elapsedSec, Level* level,const Rectf& characterShape) = 0;

	bool IsOverlapping(const Rectf& character);
	const Rectf& GetShape();
	int GetDamage()const;
	bool GetIsFinished()const;

protected:

	Rectf m_ProjectileShape;
	Vector2f m_ProjectileVelocity;
	Vector2f m_ProjectileAcceleration;
	
	float m_MoveSpeed;
	int m_Damage;
	bool m_isFinished;

};

