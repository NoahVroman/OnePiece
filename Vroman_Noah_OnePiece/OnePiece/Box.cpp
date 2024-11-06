#include "pch.h"
#include "Box.h"
#include <utils.h>
#include "Sprite.h"
#include <iostream>
#include "Level.h"
#include "PickUpManager.h"
#include "SoundManager.h"
#include <SoundEffect.h>
#include "Apple.h"
Box::Box(const Rectf& pos, TextureManager* pTextureManager,PickUpManager* pPickUpManager)
	:GameObject(pos)
	, m_BoxVelocity{ Vector2f{0,0} }
	, m_BoxAcceleration{ Vector2f{0,-2000} }
	, IsLoaded{ false }

{
	const float scale{ 4.f };
	const std::string box{ "Box.png" };
	const std::string HitBox{ "BoxHit.png" };

	m_pTempTextureManager = pTextureManager;
	m_pPickUpManager = pPickUpManager;

	m_BoxRect.left = m_GameObjectPos.left;
	m_BoxRect.bottom = m_GameObjectPos.bottom;

	m_pBox = new Sprite{ box,1,1,0,Point2f{m_BoxRect.left,m_BoxRect.bottom},scale,pTextureManager};
	m_pHitBox = new Sprite{ HitBox,5,1,12,Point2f{0,0}, scale, pTextureManager};


	m_BoxRect.width = m_pHitBox->GetFrameWidth()*1.85f;
	m_BoxRect.height = m_pHitBox->GetFrameHeight()*1.85f;

};

Box::~Box()
{
	delete m_pBox;
	m_pBox = nullptr;
	delete m_pHitBox;
	m_pHitBox = nullptr;
}

void Box::Draw() const
{ 
	glPushMatrix();
	{
		if (!m_isHit)
		{
			m_pBox->Draw();
		}
		else
		{
			if (!m_pHitBox->IsOnLastFrame())
			{
				m_pHitBox->Draw();
			}
		}

	}
	glPopMatrix();

}

void Box::Update(float elapsedSec,Level* level,PickUpManager* pPickUpManager)
{
	const float scale{ 4.f };
	bool isEnemyOnGround = level->isOnGround(m_BoxRect, m_BoxVelocity);

	m_pBox->SetDrawPos(Point2f{ m_BoxRect.left, m_BoxRect.bottom });
	m_pHitBox->SetDrawPos(Point2f{ m_BoxRect.left - (m_pHitBox->GetFrameWidth() * scale) / 4.f, m_BoxRect.bottom - (m_pHitBox->GetFrameHeight() * scale) / 4.f });

	

	if (!isEnemyOnGround)
	{
		m_BoxVelocity += m_BoxAcceleration * elapsedSec;
		m_BoxRect.bottom += m_BoxVelocity.y * elapsedSec;

	}
	if (m_isHit)
	{
		if (!IsLoaded)
		{
			m_pPickUpManager->AddPickup(Point2f{ m_BoxRect.left + (m_BoxRect.width / 2.f) - 25.f,m_BoxRect.bottom },m_pTempTextureManager);
			IsLoaded = true;
		}


		if (!m_pHitBox->IsOnLastFrame())
		{

			m_pHitBox->Update(elapsedSec);
		}

	}

}

bool Box::IsFinished()
{
	if (m_pHitBox->IsOnLastFrame())
	{
		return true;
	}
	return false;
}

const Rectf& Box::GetObjectShape() const
{
	return m_BoxRect;
}

