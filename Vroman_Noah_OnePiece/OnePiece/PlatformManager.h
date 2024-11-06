#pragma once
#include "Platform.h"
class PlatformManager final
{
public:
	PlatformManager();
	~PlatformManager();

	PlatformManager(const PlatformManager& other) = delete;
	PlatformManager& operator=(const PlatformManager& rhs) = delete;
	PlatformManager(PlatformManager&& other) = delete;
	PlatformManager& operator=(PlatformManager&& rhs) = delete;

	void PushBack(const Rectf& pos);

	void ClearPlatforms();

	void HandleAllPlatformCollision(Rectf& shapeCharacter, Vector2f& velocityCharacter);
	bool isPlayerOnGround(const Rectf& ShapeAvatar, const Vector2f& VelocityAvatar) const;


private:

	std::vector<Platform*> m_pVectorOfPlatforms;


};

