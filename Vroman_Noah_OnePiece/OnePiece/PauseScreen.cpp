#include "pch.h"
#include "PauseScreen.h"
#include "Sprite.h"
#include "InputManager.h"
#include "SoundManager.h"
#include <utils.h>
#include <SoundEffect.h>
PauseScreen::PauseScreen(TextureManager* textureManager, InputManager* inputManager, SoundManager* soundManager, const Window& window)
	:m_EndGame{false}
	,m_Continue{false}
{
	std::string PausePath{ "PauseButton.png" };
	m_pPauseScreen = new Sprite( PausePath,2,1,0,Point2f{0,0},5.f,textureManager );
	m_Window.width = window.width;
	m_Window.height = window.height;
	m_pSelectSound = soundManager->GetSoundEffectPointer("Select.mp3");
	m_pInputManager = inputManager;
}

PauseScreen::~PauseScreen()
{
	delete m_pPauseScreen;
	m_pPauseScreen = nullptr;

}

void PauseScreen::Draw() const
{
	utils::SetColor(Color4f{ 0,0,0,0.5f });
	utils::FillRect(m_Window);
	m_pPauseScreen->SetDrawPos(Point2f{ (m_Window.width / 2.f) - ((m_pPauseScreen->GetFrameWidth()/2)*5.f),(m_Window.height /2.f) - (m_pPauseScreen->GetFrameHeight() / 2.f)*5.f });
	m_pPauseScreen->Draw();
}

void PauseScreen::Update(float elapsedSec)
{
	if (m_pInputManager->IsKeyPressed(SDL_SCANCODE_SPACE))
	{
		if (m_pPauseScreen->GetCurrentFrame() == 0)
		{
			m_Continue = true;
			m_EndGame = false;
		}
		else if (m_pPauseScreen->GetCurrentFrame() == 1)
		{
			m_Continue = false;
			m_EndGame = true;
		}

	}

	if (m_pPauseScreen->GetCurrentFrame() == 0)
	{
		if (m_pInputManager->IsKeyPressed(SDL_SCANCODE_DOWN) || m_pInputManager->IsKeyPressed(SDL_SCANCODE_UP))
		{
			m_pSelectSound->Play(0);
			m_pPauseScreen->SetCurrentFrame(1);
		}
	}
	else if (m_pPauseScreen->GetCurrentFrame() == 1)
	{

		if (m_pInputManager->IsKeyPressed(SDL_SCANCODE_DOWN) || m_pInputManager->IsKeyPressed(SDL_SCANCODE_UP))
		{
			m_pSelectSound->Play(0);
			m_pPauseScreen->SetCurrentFrame(0);
		}
	}


}

bool PauseScreen::IsGameEnded()const
{
	return m_EndGame;
}

bool PauseScreen::Continue()const
{
	return m_Continue;
}

void PauseScreen::SetContinue(bool Conitune)
{
	m_Continue = Conitune;
}
