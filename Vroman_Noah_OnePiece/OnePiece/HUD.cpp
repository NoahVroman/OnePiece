#include "pch.h"
#include "HUD.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Character.h"
#include "BaseEnemyClass.h"
#include <utils.h>
#include "AxeMorgan.h"

HUD::HUD(const Window& window, TextureManager* pTextureManager)
	:m_ColorHud{1,1,0,1}
	,hitHealthBar{false}
	,m_Window{window}
	,m_isBossDead{ false }
	,m_DeathTimer{0}
	,m_MaxDeathTimer{1.f}
	,m_OriginalDeadTime{1.f}
	,m_Dead{ false }
	,m_isPlayerDead{ false }
	,m_CharacterDeathTimer{ 0.f}
	,m_MaxCharacterDeathTimer{1.f}
	,m_OriginalCharacterDeadTime{1.f}
	,m_CharacterDead{ false }

{
	m_pHUDSprite = new Sprite("hud.png", 1, 1, 0, Point2f{ 0,0 }, 5.f, pTextureManager);
	m_pBossHUD = new Sprite("BossHUD.png", 1, 1, 0, Point2f{ 0,0 }, 5.f, pTextureManager);

	m_HealthBarRect.left = 158.f;
	m_HealthBarRect.bottom = (m_pHUDSprite->GetFrameHeight() *5) - 107.5f;
	m_HealthBarRect.width = 325.0f;
	m_HealthBarRect.height = 30.f;

	m_FadedHealthBar.left = m_HealthBarRect.left;
	m_FadedHealthBar.bottom = m_HealthBarRect.bottom;
	m_FadedHealthBar.width = m_HealthBarRect.width;
	m_FadedHealthBar.height = m_HealthBarRect.height;


	m_BossHealthBar.left = window.width - (m_pBossHUD->GetFrameWidth()*5.f) + 32.f;
	m_BossHealthBar.bottom = (m_pBossHUD->GetFrameHeight() * 5) - 107.5f;
	m_BossHealthBar.width = 325.f;
	m_BossHealthBar.height = 30.f;

	
	m_FadedBossHealthBar.left = m_BossHealthBar.left;
	m_FadedBossHealthBar.bottom = m_BossHealthBar.bottom;
	m_FadedBossHealthBar.width = m_BossHealthBar.width;
	m_FadedBossHealthBar.height = m_BossHealthBar.height;

}

HUD::~HUD()
{
	delete m_pHUDSprite;
	m_pHUDSprite = nullptr;
	delete m_pBossHUD;
	m_pBossHUD = nullptr;
}

void HUD::Draw() const
{

	m_pHUDSprite->Draw();
	utils::SetColor(m_ColorHud);
	utils::FillRect(m_HealthBarRect);

	utils::SetColor(Color4f{1,1,0,0.7f});
	utils::FillRect(m_FadedHealthBar);


}

void HUD::Update(float elapsedSec, Character* pPlayer)
{
	float healthBarWidth = (pPlayer->GetHealth() / 100.f) * 1300.f * 2.53f;
	m_HealthBarRect.width = healthBarWidth;

	if (m_FadedHealthBar.width != m_HealthBarRect.width)
	{
		m_FadedHealthBar.width -= 100 * elapsedSec;

		if (m_FadedHealthBar.width < m_HealthBarRect.width)
		{
			m_FadedHealthBar.width = m_HealthBarRect.width;
		}
	}

	if (pPlayer->GetHealth() <= 0)
	{
		m_CharacterDead = true;
	}
	if (m_CharacterDead && m_CharacterDeathTimer <= m_MaxCharacterDeathTimer)
	{
		m_CharacterDeathTimer += elapsedSec;
	}

	if (m_CharacterDeathTimer > m_MaxCharacterDeathTimer)
	{
		m_isPlayerDead = true;
		m_CharacterDeathTimer = 0.f;
		m_MaxCharacterDeathTimer = m_OriginalCharacterDeadTime;
	}

}

void HUD::DrawBossHUD() const
{
	m_pBossHUD->SetDrawPos(Point2f{ m_Window.width - m_pBossHUD->GetFrameWidth() * 5.0f,0 });

	m_pBossHUD->Draw();

	utils::SetColor(m_ColorHud);
	utils::FillRect(m_BossHealthBar);

	//utils::SetColor(Color4f{ 1,1,0,0.7f });
	//utils::FillRect(m_FadedBossHealthBar);

}

void HUD::UpdateBoss(float elapsedSec, AxeMorgan* enemy)
{
	float healthBarWidth = (enemy->GetHealth() / 100.f) * 1300.f * 2.6f;
	m_BossHealthBar.width = healthBarWidth ;
	m_BossHealthBar.left =  (m_Window.width - m_BossHealthBar.width) - 150.f;

	if (enemy->GetHealth() <= 0)
	{
		m_Dead = true;
	}


	if (m_Dead && m_DeathTimer <= m_MaxDeathTimer)
	{
		m_DeathTimer += elapsedSec;
	}

	if (m_DeathTimer > m_MaxDeathTimer)
	{
		m_isBossDead = true;
		m_DeathTimer = 0.f;
		m_MaxDeathTimer = m_OriginalDeadTime;
	}

}

bool HUD::GetBossDead() const
{
	return m_isBossDead;
}

bool HUD::GetPlayerDead() const
{
	return m_isPlayerDead;
}
