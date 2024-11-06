#include "pch.h"
#include "Game.h"
#include "Level.h"
#include "TextureManager.h"
#include "LoadingScreen.h"
#include "StartScreen.h"
#include "PauseScreen.h"
#include "InputManager.h"
#include "SoundManager.h"
#include <SoundStream.h>
#include <SoundEffect.h>
#include "EndScreen.h"
#include "DeathScreen.h"
#include <iostream>
#include <fstream>
Game::Game( const Window& window ) 
	:BaseGame{ window }
	,m_Window{window}
{

	Initialize(window);
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize(const Window& window)
{
	m_FadeRect.left = 0;
	m_FadeRect.bottom = 0;
	m_FadeRect.width = m_Window.width;
	m_FadeRect.height = m_Window.height;

	m_GameState = GameState::loadingScreen;
	m_IsPlaying = false;
	m_IsInPause = false;
	m_IsInEndScreen = false;
	m_FadeColor = Color4f{ 0,0,0,0 };
	m_HasHitMaxAlpha = false;
	m_Restart = false;
	m_Continue = false;

	m_pTextureManager = new TextureManager();
	m_pSoundManager = new SoundManager();
	m_pInputManager = new InputManager();
	m_pLevel = new Level(m_Window,m_pTextureManager,m_pSoundManager,m_pInputManager);
	m_pLoadingScreen = new LoadingScreen(m_pTextureManager,m_Window);
	m_pStartScreen = new StartScreen(m_pTextureManager,m_pInputManager, m_Window,m_pSoundManager);
	m_pPauseScreen = new PauseScreen(m_pTextureManager, m_pInputManager,m_pSoundManager, m_Window);
	m_pEndScreen = new EndScreen(m_pTextureManager);
	m_pDeathScreen = new DeathScreen(m_pTextureManager, m_pInputManager, m_pSoundManager, m_Window);
	m_pIntroScreenMusic = m_pSoundManager->GetSoundStreamPointer("IntroMusic.mp3");
	m_pGameMusic = m_pSoundManager->GetSoundStreamPointer("Music.mp3");
	m_pEndScreenMusic = m_pSoundManager->GetSoundStreamPointer("EndScreen.mp3");
	m_pPauseScreenEffect = m_pSoundManager->GetSoundEffectPointer("PauseKnop.wav");

}

void Game::Cleanup( )
{
	delete m_pLevel;
	m_pLevel = nullptr;

	delete m_pTextureManager;
	m_pTextureManager = nullptr;

	delete m_pInputManager;
	m_pInputManager = nullptr;
	delete m_pLoadingScreen;
	m_pLoadingScreen = nullptr;
	delete m_pStartScreen;
	m_pStartScreen = nullptr;
	delete m_pSoundManager;
	m_pSoundManager = nullptr;
	delete m_pPauseScreen;
	m_pPauseScreen = nullptr;

	delete m_pEndScreen;
	m_pEndScreen = nullptr;

	delete m_pDeathScreen;
	m_pDeathScreen = nullptr;
}

void Game::Update( float elapsedSec )
{

	if (m_GameState == GameState::loadingScreen)
	{
		if (m_pLoadingScreen->HasFinished())
		{
			m_GameState = GameState::startScreen;
		}

	}
	if ((m_GameState == GameState::startScreen) || (m_GameState == GameState::play))
	{
		if (m_pStartScreen->isFinished())
		{
			FadeInOut(elapsedSec);

		}

	}



	switch (m_GameState)
	{
	case Game::GameState::loadingScreen:
		if (!m_pIntroScreenMusic->IsPlaying())
		{
			m_pIntroScreenMusic->Play(true);

		}
		m_pLoadingScreen->Update(elapsedSec);
		break;
	case Game::GameState::startScreen:
		m_pStartScreen->Update(elapsedSec);
		break;
	case Game::GameState::play:
		m_pPauseScreen->SetContinue(false);
		if (!m_IsPlaying)
		{
			if (m_pIntroScreenMusic->IsPlaying())
			{
				m_pIntroScreenMusic->Stop();
			}
		    m_IsPlaying = true;

		}

		if (m_pGameMusic->IsPlaying() == false)
		{
			
			m_pGameMusic->Play(true);
		}

		m_pLevel->UpdateLevel(elapsedSec,m_pTextureManager);


		break;
	case Game::GameState::pauseScreen:

		m_pPauseScreen->Update(elapsedSec);

		if (m_pPauseScreen->IsGameEnded())
		{
			m_GameState = GameState::endScreen;
		}
		else if (m_pPauseScreen->Continue())
		{
			m_GameState = GameState::play;
			m_IsInPause = false;
		}


		break;
	case Game::GameState::endScreen:
		if (!m_IsInEndScreen)
		{
			if (m_pGameMusic->IsPlaying())
			{
				m_pGameMusic->Stop();
			}
			m_IsInEndScreen = true;
		}
		if (!m_pEndScreenMusic->IsPlaying())
		{
			m_pEndScreenMusic->Play(false);

		}
		m_pEndScreen->Update(elapsedSec);
		break;
	case Game::GameState::deathScreen:
		m_pDeathScreen->Update(elapsedSec);
		break;

	}



	if (!(m_GameState == GameState::loadingScreen) && !(m_GameState == GameState::startScreen) && !(m_GameState == GameState::endScreen))
	{
		if (m_IsInPause)
		{
			
			m_GameState = GameState::pauseScreen;
		}
		else
		{
			m_GameState = GameState::play;
		}

	}




	if (m_pLevel->GetCharacterDead())
	{
		m_GameState = GameState::deathScreen;
	}
	if (m_pLevel->GetBossDead())
	{
		m_GameState = GameState::endScreen;

	}


	if (m_GameState == GameState::deathScreen)
	{
		if (m_pDeathScreen->Quit())
		{
			m_GameState = GameState::endScreen;
		}
		else if (m_pDeathScreen->Restart())
		{
			if (!m_Restart)
			{
				RestartLevel(m_Window);
				m_GameState = GameState::play;
				m_Restart = true;
			}
		}
	}




}

void Game::Draw( ) const
{
	ClearBackground( );
	switch (m_GameState)
	{
	case Game::GameState::loadingScreen:
		m_pLoadingScreen->Draw();
		break;
	case Game::GameState::startScreen:
		m_pStartScreen->Draw();
		break;
	case Game::GameState::play:
		m_pLevel->DrawLevel();
		break;
	case Game::GameState::pauseScreen:
		m_pLevel->DrawBackground();
		m_pPauseScreen->Draw();
		break;
	case Game::GameState::endScreen:
		m_pEndScreen->Draw();
		break;
	case Game::GameState::deathScreen:
		m_pDeathScreen->Draw();
		break;

	}

	utils::SetColor(m_FadeColor);
	utils::FillRect(m_FadeRect);


	
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_pLevel->PressKeyDown(e);
	std::string pathname{ "Controls.txt" };
	std::ifstream isInFile{};
	isInFile.open(pathname);

	switch (e.keysym.scancode)
	{
	case SDL_SCANCODE_I: 

		if (!isInFile.is_open())
		{
			std::cout << "ERROR OPENING FILE" << std::endl;
		}
		else
		{
			std::string line{};
			
			while (!isInFile.fail() && !isInFile.eof())
			{
				std::getline(isInFile, line);
				std::cout << line << std::endl;
			}
		}
		isInFile.close();
		break;
	case SDL_SCANCODE_SEMICOLON:
		m_pSoundManager->MuteAndUnmute();
		break;
	case SDL_SCANCODE_M:
		m_pSoundManager->MuteAndUnmute();
		break;
	case SDL_SCANCODE_ESCAPE:

		if (m_GameState == GameState::play)
		{
			m_pPauseScreenEffect->SetVolume(15);
			m_pPauseScreenEffect->Play(0);

			m_IsInPause = true;
		}
		break;
	case SDL_SCANCODE_O:
		m_pSoundManager->IncreaseEffectsVolume();
		m_pSoundManager->IncreaseStreamVolume();
		break;
	case SDL_SCANCODE_L:
		m_pSoundManager->DecreaseEffectsVolume();
		m_pSoundManager->DecreaseStreamVolume();

		break;

	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_pLevel->PressKeyUp(e);
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{

}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{

}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::FadeInOut(float elapsedSec)
{
	if (m_FadeColor.a <= 1.f && !m_HasHitMaxAlpha)
	{
		m_FadeColor.a += elapsedSec;
	}
	else
	{
		m_HasHitMaxAlpha = true;

	}
	if (m_FadeColor.a >= 0.0f && m_HasHitMaxAlpha)
	{
		m_FadeColor.a -= elapsedSec;
	}

	if (m_HasHitMaxAlpha)
	{
		m_GameState = GameState::play;
	}
}

void Game::RestartLevel(const Window& window)
{
	Cleanup();
	Initialize(window);
}
