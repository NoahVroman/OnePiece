#include "pch.h"
#include "LoadingScreen.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include <SoundStream.h>
#include <utils.h>
#include <iostream>
LoadingScreen::LoadingScreen(TextureManager* spritemanager, const Window& window)
	:m_HorizontalSpeed{ 1800.0f }
	, m_VerticalSpeed{ 300.f }
	, m_SlowTextSpeed{ 400.0f }
	, m_HasFinishedBorders{ false }
	, m_CharacterType{ CharacterType::luffy }
	, m_isFinished{ false }

{
	const std::string LoadingScreenBackground{ "LoadingScreenBackground.png" };
	const std::string Characters{ "LoadingScreenCharacters.png" };
	const std::string CharactersNames{ "LoadingScreenCharactersNames.png" };


	m_pLoadingScreenBackground = new Sprite(LoadingScreenBackground, 3, 1, 1, Point2f{ 0,0 }, 5, spritemanager);
	m_pCharacters = new Sprite(Characters, 5, 1, 0, Point2f{ 0,0 }, 5, spritemanager);
	m_pCharacterNames = new Sprite(CharactersNames, 5, 1, 0, Point2f{ 0,0 }, 5, spritemanager);

	m_LoadingScreenBackground.left = 0;
	m_LoadingScreenBackground.bottom = 0;
	m_LoadingScreenBackground.width = m_pLoadingScreenBackground->GetFrameWidth();
	m_LoadingScreenBackground.height = m_pLoadingScreenBackground->GetFrameHeight();


	m_BottomBlackBorder.left = 0;
	m_BottomBlackBorder.height = 0;
	m_BottomBlackBorder.width = window.width;
	m_BottomBlackBorder.height = window.height / 2.f;

	m_TopBlackBorder.left = 0;
	m_TopBlackBorder.bottom = window.height / 2.f;
	m_TopBlackBorder.width = window.width;
	m_TopBlackBorder.height = window.height / 2.f;

	m_Window.left = 0;
	m_Window.bottom = 0;
	m_Window.width = window.width;
	m_Window.height = window.height;

	m_Characters.left = -m_pCharacters->GetFrameWidth() * 5;
	m_Characters.bottom = 0;
	m_Characters.width = m_pCharacters->GetFrameWidth() * 5;
	m_Characters.height = m_pCharacters->GetFrameHeight() * 5;

	m_CharacterNames.left = window.width + (m_pCharacterNames->GetFrameWidth() * 5);
	m_CharacterNames.bottom = (window.height - m_pCharacterNames->GetFrameHeight() * 5) / 2.f;
	m_CharacterNames.width = m_pCharacterNames->GetFrameWidth() * 5;
	m_CharacterNames.height = m_pCharacterNames->GetFrameHeight() * 5;
}

LoadingScreen::~LoadingScreen()
{
	delete	m_pLoadingScreenBackground;
	m_pLoadingScreenBackground = nullptr;
	delete	m_pCharacters;
	m_pCharacters = nullptr;
	delete	m_pCharacterNames;
	m_pCharacterNames = nullptr;

}

void LoadingScreen::Draw() const
{
	glPushMatrix();
	{
		DrawBackground();
		DrawCharacters();

		if (m_pLoadingScreenBackground->GetCurrentFrame() > 1)
		{
			m_pCharacterNames->SetDrawPos(Point2f{ m_CharacterNames.left,m_CharacterNames.bottom });
			m_pCharacterNames->Draw();
		}
	}
	glPopMatrix();

}

void LoadingScreen::Update(float elapsedSec)
{
	UpdateBackground(elapsedSec);
	UpdateCharacters(elapsedSec);

}

bool LoadingScreen::HasFinished()
{
	return m_isFinished;
}

void LoadingScreen::DrawBackground() const
{
	m_pLoadingScreenBackground->Draw();

	if (m_pLoadingScreenBackground->GetCurrentFrame() > 1)
	{
		utils::SetColor(Color4f{ 0,0,0,1 });
		utils::FillRect(m_BottomBlackBorder);
		utils::FillRect(m_TopBlackBorder);
	}

}

void LoadingScreen::UpdateBackground(float elapsedsec)
{
	float maxBottomHeight{ m_Window.height * 0.2f };
	float maxTopPos{ m_Window.height * 0.8f };

	if (!m_pLoadingScreenBackground->IsOnLastFrame())
	{
		m_pLoadingScreenBackground->Update(elapsedsec);
	}

	if (m_pLoadingScreenBackground->GetCurrentFrame() > 1)
	{
		if (m_BottomBlackBorder.height > maxBottomHeight)
		{
			m_BottomBlackBorder.height -= m_VerticalSpeed * elapsedsec;
		}
		else
		{
			m_HasFinishedBorders = true;

		}
		if (m_TopBlackBorder.bottom < maxTopPos)
		{
			m_TopBlackBorder.bottom += m_VerticalSpeed * elapsedsec;
		}
		else
		{
			m_HasFinishedBorders = true;

		}

	}

}

void LoadingScreen::DrawCharacters() const
{
	if (m_pLoadingScreenBackground->GetCurrentFrame() > 1)
	{
		m_pCharacters->SetDrawPos(Point2f{ m_Characters.left,m_Characters.bottom });
		m_pCharacters->Draw();
	}

}

void LoadingScreen::UpdateCharacters(float elapsedsec)
{
	int myInt = static_cast<int>(m_CharacterType); // Convert enum value to integer

	if (m_pLoadingScreenBackground->GetCurrentFrame() > 1)
	{
		if (!m_isFinished)
		{

			if (m_HasFinishedBorders)
			{
				m_pCharacters->Update(elapsedsec);
				if (m_Characters.left < -5.f)
				{
					m_Characters.left += m_HorizontalSpeed * elapsedsec;
				}
			}

			if (m_HasFinishedBorders)
			{
				if (m_CharacterNames.left > 0 - m_CharacterNames.width)
				{
					if (m_CharacterNames.left >= m_Window.width * 0.5f && m_CharacterNames.left < m_Window.width - (m_Window.width / 3.f))
					{
						m_CharacterNames.left -= m_SlowTextSpeed * elapsedsec;

					}
					else
					{
						m_CharacterNames.left -= m_HorizontalSpeed * elapsedsec;
					}
				}
				else
				{
					++myInt;
					m_CharacterType = static_cast<CharacterType>(myInt);
					m_pCharacters->SetCurrentFrame(myInt);
					m_pCharacterNames->SetCurrentFrame(myInt);
					ResetPostions();
					if (myInt > 4)
					{
						m_isFinished = true;
					}
				}
			}

		}

	}




}

void LoadingScreen::ResetPostions()
{
	m_Characters.left = -m_pCharacters->GetFrameWidth() * 5;
	m_Characters.bottom = 0;
	m_Characters.width = m_pCharacters->GetFrameWidth() * 5;
	m_Characters.height = m_pCharacters->GetFrameHeight() * 5;

	m_CharacterNames.left = m_Window.width + (m_pCharacterNames->GetFrameWidth() * 5);
	m_CharacterNames.bottom = (m_Window.height - m_pCharacterNames->GetFrameHeight() * 5) / 2.f;
	m_CharacterNames.width = m_pCharacterNames->GetFrameWidth() * 5;
	m_CharacterNames.height = m_pCharacterNames->GetFrameHeight() * 5;

}
