#include "pch.h"
#include "DeathScreen.h"
#include "InputManager.h"
#include "Sprite.h"
#include "TextureManager.h"

DeathScreen::DeathScreen(TextureManager* spritemanager, InputManager* inputManager, SoundManager* soundManager, const Window& window)
	:m_pInputmanager{inputManager}
	,m_Restart{false}
	,m_Quit{ false }
	,m_Window{window}
{

	m_pDeathScreen = new Sprite("DeathScreen.png", 2, 1, 0, Point2f{ 0,0 }, 5.f, spritemanager);

	
}

DeathScreen::~DeathScreen()
{
	delete m_pDeathScreen;
	m_pDeathScreen = nullptr;
}

void DeathScreen::Draw() const
{
	m_pDeathScreen->Draw();
}

void DeathScreen::Update(float elapsedSec)
{
	if (m_pInputmanager->IsKeyPressed(SDL_SCANCODE_SPACE))
	{
		if (m_pDeathScreen->GetCurrentFrame() == 0) 
		{
			m_Restart = false;
			m_Quit = true;
		}
		if (m_pDeathScreen->GetCurrentFrame() == 1)
		{
			m_Restart = true;
			m_Quit = false;
		}
	}
	

	if (m_pDeathScreen->GetCurrentFrame() == 0)
	{
		if (m_pInputmanager->IsKeyPressed(SDL_SCANCODE_RIGHT) || m_pInputmanager->IsKeyPressed(SDL_SCANCODE_LEFT))
		{
			m_pDeathScreen->SetCurrentFrame(1);
		}
	}
	else if (m_pDeathScreen->GetCurrentFrame() == 1)
	{
		if (m_pInputmanager->IsKeyPressed(SDL_SCANCODE_RIGHT) || m_pInputmanager->IsKeyPressed(SDL_SCANCODE_LEFT))
		{
		
			m_pDeathScreen->SetCurrentFrame(0);

		}

	}
}

bool DeathScreen::Restart() const
{
	return m_Restart;
}

bool DeathScreen::Quit() const
{
	return m_Quit;
}
