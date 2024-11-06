#include "pch.h"
#include "Camera.h"

Camera::Camera(float width, float height)
	:m_Width{ width }
	, m_Height{ height }
	, m_LevelBoundries{ Rectf{0,0,0, 0} }
	, m_StateCamera{ CameraState::moving }
{

}

void Camera::Transform(const Rectf& target) const
{
	Point2f pos{ Track(target) };
	Clamp(pos);
	glTranslatef(-pos.x, -pos.y, 0);
}

void Camera::SetLevelBoundries(const Rectf& levelBoundries)
{
	m_LevelBoundries.height = levelBoundries.height;
	m_LevelBoundries.width = levelBoundries.width;

}

Point2f Camera::Track(const Rectf& Target) const
{
	Point2f outCome{ Target.left + Target.width / 2 - m_Width / 2, Target.bottom + Target.height / 2 - m_Height / 2 };
	return outCome;
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	if (bottomLeftPos.x < 0)
	{
		bottomLeftPos.x = 0;
	}
	if (bottomLeftPos.y < 0)
	{
		bottomLeftPos.y = 0;
	}
	if (bottomLeftPos.x > m_LevelBoundries.width - m_Width)
	{
		bottomLeftPos.x = m_LevelBoundries.width - m_Width;
	}
	if (bottomLeftPos.y > m_LevelBoundries.height - m_Height)
	{
		bottomLeftPos.y = m_LevelBoundries.height - m_Height;
	}

}

Point2f Camera::GetCameraTransform(const Rectf& target)
{
	if (m_StateCamera == CameraState::moving)
	{
		Point2f pos{ Track(target) };
		Clamp(pos);
		m_PosCamera.x = pos.x;

	}
	return m_PosCamera;
}

float Camera::GetCameraWidth() const
{
	return m_Width;
}

float Camera::GetCameraHeight() const
{
	return m_Height;
}

void Camera::SetCurrentCameraStateMoving()
{
	m_StateCamera = CameraState::moving;

}

void Camera::SetCurrentCameraStateWaiting()
{
	m_StateCamera = CameraState::waiting;

}


