#pragma once
#include <Vector2f.h>
#include <vector>
class TextureManager;
class Projectile;
class Level;
class SoundManager;
class ProjectileManager final
{
public:
	ProjectileManager();
	~ProjectileManager();

	enum class Type
	{
		blade,
		bomb,
	};

	ProjectileManager(const ProjectileManager& other) = delete;
	ProjectileManager& operator=(const ProjectileManager& rhs) = delete;
	ProjectileManager(ProjectileManager&& other) = delete;
	ProjectileManager& operator=(ProjectileManager&& rhs) = delete;

	void AddProjectile(const Type& type,const Point2f& position, TextureManager* pTextureManager, const Vector2f& velocity, SoundManager* pSoundManager);

	void PopProjectile();

	void ClearProjectile();

	void Draw()const;
	void Update(float elapsedSec, Level* level,const Rectf& characterShape);
	bool IsOverlapping(const Rectf& shape);
	const Rectf& GetProjectileShape();

	int GetDamage() const;
	bool GetFinished()const;

private:

	std::vector<Projectile*> m_pProjectileVector;

};

