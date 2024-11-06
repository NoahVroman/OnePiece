#pragma once
class TextureManager;
class Character;
class Sprite;
class BaseEnemyClass;
class AxeMorgan;
class HUD
{
public:
	HUD(const Window& window, TextureManager* pTextureManager);
	~HUD();

	HUD(const HUD& other) = delete;
	HUD& operator=(const HUD& rhs) = delete;
	HUD(HUD&& other) = delete;
	HUD& operator=(HUD&& rhs) = delete;

	void Draw() const;
	void Update(float elapsedSec,Character* pPlayer);

	void DrawBossHUD() const;
	void UpdateBoss(float elapsedSec, AxeMorgan* enemy);
	
	bool GetBossDead()const;
	bool GetPlayerDead()const;


private:

	const Color4f m_ColorHud;
	Rectf m_HealthBarRect;
	Rectf m_FadedHealthBar;

	bool hitHealthBar;

	bool m_isBossDead;
	bool m_isPlayerDead;

	const Window& m_Window;

	Sprite* m_pHUDSprite;
	Sprite* m_pBossHUD;

	Rectf m_BossHealthBar;
	Rectf m_FadedBossHealthBar;

	bool m_Dead;
	float m_DeathTimer;
	float m_MaxDeathTimer;
	const float m_OriginalDeadTime;


	bool m_CharacterDead;
	float m_CharacterDeathTimer;
	float m_MaxCharacterDeathTimer;
	const float m_OriginalCharacterDeadTime;



};

