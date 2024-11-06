#pragma once
#include "Projectile.h"
class TextureManager;
class Sprite;
class Level;
class Blade final : public Projectile
{

public:

	Blade(const Point2f& position,TextureManager* pTextureManager, const Vector2f& velocity);
	~Blade();

	Blade(const Blade& other) = delete;
	Blade& operator=(const Blade& rhs) = delete;
	Blade(Blade&& other) = delete;
	Blade& operator=(Blade&& rhs) = delete;

	virtual void Draw()const;
	virtual void Update(float elapsedSec, Level* level, const Rectf& characterShape);


private:


	//////////////
	/// Sprite ///
	//////////////
	Sprite* m_pBladeProjectile;
	Vector2f m_OtherVelocity;



};

