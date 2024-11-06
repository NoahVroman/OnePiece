#include "pch.h"
#include "Apple.h"
#include "Sprite.h"
#include "TextureManager.h"
#include <utils.h>
Apple::Apple(const Point2f& pos, TextureManager* pTextureManager)
	:m_isHit{false}
	,isFinished{ false }
	,m_IsCollected{ false }
	, m_End{ false }
	, m_DeathTimer{ 0.0f }
	, m_MaxDeathTimer{ 1.f }
	, m_OriginalDeathTime{ 1.f }

	
{
	float scale{ 4 };
	m_Shape.left = pos.x;
	m_Shape.bottom = pos.y;
	m_Shape.height = 50.f;
	m_Shape.width = 50.f;

	const float offset{ 200.0f };
	m_EndPoint = Point2f{ m_Shape.left,m_Shape.bottom + offset };

	m_pApple = new Sprite("Apple.png", 1, 1, 1, Point2f{0,0}, scale, pTextureManager);

}
Apple::~Apple()
{
	delete m_pApple;
	m_pApple = nullptr;
}
void Apple::Draw() const
{
	m_pApple->SetDrawPos(Point2f{ m_Shape.left,m_Shape.bottom });
	m_pApple->Draw();
}

void Apple::Update(float elapsedSec)
{
	if (m_isHit) 
	{
		if (m_Shape.bottom <= m_EndPoint.y)
		{
			m_Shape.bottom += 1500.f * elapsedSec;
		}
		else
		{
			m_End = true;
		}
	}

	if (m_End && m_DeathTimer <= m_MaxDeathTimer)
	{
		m_DeathTimer += elapsedSec;
	}

	if (m_DeathTimer > m_MaxDeathTimer)
	{
		m_End = false;
		isFinished = true;
		m_DeathTimer = 0.f;
		m_MaxDeathTimer = m_OriginalDeathTime;
	}

}

bool Apple::IsOverlapping(const Rectf& shape)
{
	if (!m_IsCollected && utils::IsOverlapping(m_Shape, shape))
	{
		m_isHit = true;
		return true;
	}
	return false;
}

bool Apple::IsFinished()
{
	return isFinished;
}

void Apple::SetCollected(bool collected)
{
	m_IsCollected = collected;
}

bool Apple::GetCollected()const
{
	return m_IsCollected;
}

const Rectf& Apple::GetShape()const
{
	return m_Shape;
}

