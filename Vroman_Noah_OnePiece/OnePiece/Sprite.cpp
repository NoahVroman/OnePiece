#include "pch.h"
#include "Sprite.h"
#include "Texture.h"
#include "TextureManager.h"


Sprite::Sprite(const std::string& filename, int nrCols, int nrRows, int framesPerSec, const Point2f& drawPos, float scale, TextureManager* pTextureManager)
	: m_FileName{filename}
	, m_Columns{ nrCols }
	, m_Rows{ nrRows }
	, m_FrameRate{ framesPerSec }
	, m_DrawPos{ drawPos }
	, m_TotalFrames{ nrRows * nrCols } //-1
	, m_CurrentFrame{ 0 }
	, m_AccumulatedSec{ 0.f }
	, m_Scale{ scale }
	, m_IsPlayingBackwards{ false }
{
	m_pTexture = pTextureManager->GetSpritePointer(filename);

	m_FrameWidth = (m_pTexture->GetWidth() / m_Columns);
	m_FrameHeight = (m_pTexture->GetHeight() / m_Rows);
}


Sprite::~Sprite()
{

}


void Sprite::Update(float elapsedSec)
{
	if (!m_IsPlayingBackwards)
	{
		m_AccumulatedSec += elapsedSec;
		if (m_AccumulatedSec >= 1.f / m_FrameRate)
		{
			++m_CurrentFrame;
			m_AccumulatedSec -= 1.f / m_FrameRate;
		}
		if (m_CurrentFrame >= m_TotalFrames)
		{
			m_CurrentFrame = 0;
		}
	}
	else
	{
		PlayBackwards(elapsedSec);
	}

}

void  Sprite::Draw() const
{
	
	Rectf sourceRect{};
	sourceRect.left		= (m_CurrentFrame % m_Columns) * (m_FrameWidth);
	sourceRect.bottom	= (m_CurrentFrame / m_Columns + 1) * (m_FrameHeight);
	sourceRect.width	= m_FrameWidth;
	sourceRect.height	= m_FrameHeight;

	Rectf destRect{m_DrawPos.x,m_DrawPos.y,sourceRect.width * m_Scale,sourceRect.height * m_Scale};
	m_pTexture->Draw(destRect, sourceRect);
}

float Sprite::GetFrameWidth() const
{
	return m_FrameWidth;
}

float Sprite::GetFrameHeight() const
{
	return m_FrameHeight;
}

void Sprite::SetDrawPos(const Point2f& newPos)
{
	m_DrawPos = newPos;
}

void Sprite::SetCurrentFrame(int newframe)
{
	m_CurrentFrame = newframe;
}

void Sprite::SetFramerate(int newframerate)
{
	m_FrameRate = newframerate;
}

void Sprite::HoldLastFrame()
{
	if (m_CurrentFrame >= m_TotalFrames - 1)
	{
		m_CurrentFrame = m_TotalFrames - 1;
	}

}

bool Sprite::IsOnLastFrame()
{
	return m_CurrentFrame >= m_TotalFrames - 1;
}

int Sprite::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

int Sprite::GetTotalFrames() const
{
	return m_TotalFrames;
}

bool Sprite::IsPlayingBackwards() const
{
	return m_IsPlayingBackwards;
}

void Sprite::SetPlayingBackwards(bool isPlayingBackwards)
{
	m_IsPlayingBackwards = isPlayingBackwards;
}

void Sprite::PlayBackwards(float elapsedSec)
{
	m_AccumulatedSec += elapsedSec;
	if (m_AccumulatedSec >= 1.f / m_FrameRate)
	{
		if (m_CurrentFrame == 0)
		{
			m_IsPlayingBackwards = false;
			++m_CurrentFrame;
		}
		else
		{
			--m_CurrentFrame;
		}
		m_AccumulatedSec -= 1.f / m_FrameRate;
	}
}
