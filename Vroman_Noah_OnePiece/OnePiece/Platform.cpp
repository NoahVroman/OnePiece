#include "pch.h"
#include "Platform.h"
#include <utils.h>
Platform::Platform(const Rectf& shape)
{


	m_Vertices.push_back(Point2f{ shape.left,shape.bottom });
	m_Vertices.push_back(Point2f{ shape.left + shape.width,shape.bottom });
	m_Vertices.push_back(Point2f{ shape.left + shape.width,shape.bottom + shape.height });
	m_Vertices.push_back(Point2f{ shape.left,shape.bottom + shape.height });
	m_Vertices.push_back(Point2f{ shape.left,shape.bottom });
}

Platform::~Platform()
{
}


void Platform::HandleCollision(Rectf& shapeCharacter, Vector2f& velocityCharacter)
{
	if (velocityCharacter.y > 0) return;
	utils::HitInfo HI{};
	Point2f p1{ shapeCharacter.left + shapeCharacter.width / 2,shapeCharacter.bottom + shapeCharacter.height * 0.2f };
	Point2f p2{ shapeCharacter.left + shapeCharacter.width / 2,shapeCharacter.bottom };

	if (Raycast(m_Vertices, p1, p2, HI))
	{
		velocityCharacter.y = 0;
		shapeCharacter.bottom = HI.intersectPoint.y;
	}
}

bool Platform::isOnGround(const Rectf& ShapeAvatar, const Vector2f& VelocityAvatar)const
{
	utils::HitInfo HI{};
	if (VelocityAvatar.y > 0) return false;
	else if (Raycast(m_Vertices, Point2f{ ShapeAvatar.left + ShapeAvatar.width / 2, ShapeAvatar.bottom - 1 }, Point2f{ ShapeAvatar.left + ShapeAvatar.width / 2, ShapeAvatar.bottom + ShapeAvatar.height * 0.2f }, HI))
	{
		return true;
	}
	return false;
}
