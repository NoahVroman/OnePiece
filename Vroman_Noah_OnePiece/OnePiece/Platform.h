#pragma once
#include "GameObject.h"
#include <Vector2f.h>
#include <vector>
class Platform final
{
public:

	Platform(const Rectf& pos);
	~Platform();

	Platform(const Platform& other) = delete;
	Platform& operator=(const Platform& rhs) = delete;
	Platform(Platform&& other) = delete;
	Platform& operator=(Platform&& rhs) = delete;

	void HandleCollision(Rectf& shapeCharacter, Vector2f& velocityCharacter);
	bool isOnGround(const Rectf& ShapeAvatar, const Vector2f& VelocityAvatar)const;

private:
	std::vector<Point2f> m_Vertices;
	Rectf m_ShapePlatform;
};

