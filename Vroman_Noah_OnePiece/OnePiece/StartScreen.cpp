#include "pch.h"
#include "StartScreen.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "InputManager.h"
#include "SoundManager.h"
#include <SoundEffect.h>

StartScreen::StartScreen(TextureManager* spritemanager, InputManager* inputManager, const Window& window, SoundManager* pSoundManager)
	:m_StartScreenState{ StartScreenState::startscreen }
	, m_VerticalSpeed{ 30.f }
	, m_hitMaxHeight{ false }
	, m_isFinished{ false }
{
	const std::string Background{ "StartScreenBackground.png" };
	const std::string Boat{ "Boat.png" };
	const std::string Text{ "StartScreenText.png" };
	const std::string Water{ "Water.png" };
	const std::string Logo{ "OnePiece.png" };
	const std::string SelectData{ "SelectData.png" };
	const std::string FileSelect{ "FileSelect.png" };
	const std::string Sword{ "Sword.png" };


	m_pStartScreenBackground = new Sprite(Background, 2, 1, 1, Point2f{ 0,0 }, 5, spritemanager);
	m_pBoat = new Sprite(Boat, 4, 1, 4, Point2f{ 0,0 }, 5, spritemanager);
	m_pText = new Sprite(Text, 1, 1, 0, Point2f{ 10,10 }, 5, spritemanager);
	m_pWater = new Sprite(Water, 1, 1, 1, Point2f{ 0,0 }, 5, spritemanager);
	m_pLogo = new Sprite(Logo, 1, 1, 1, Point2f{ 10,window.height * 0.6f }, 5, spritemanager);
	m_pSelectData = new Sprite(SelectData, 1, 1, 1, Point2f{ ((window.width / 2.f) - (window.width * 0.2f)),window.height - (window.height * 0.1f) }, 0.5f, spritemanager);
	m_pFileSelect = new Sprite(FileSelect, 2, 1, 0, Point2f{ window.width * 0.08f,(window.height / 2.f) - 100.f }, 0.7f, spritemanager);
	m_pSword = new Sprite(Sword, 1, 1, 0, Point2f{ 0,0 }, 1, spritemanager);

	m_Sword.left = 0;
	m_Sword.bottom = (window.height / 2.f);
	m_Sword.width = m_pSword->GetFrameWidth();
	m_Sword.height = m_pSword->GetFrameHeight();

	m_pSword->SetDrawPos(Point2f{ m_Sword.left,m_Sword.bottom });



	m_pFileSelect->SetCurrentFrame(1);

	m_Boat.bottom = 0;
	m_Boat.left = window.width - (m_pBoat->GetFrameWidth() * 5);
	m_Boat.width = m_pBoat->GetFrameWidth() * 5;
	m_Boat.height = m_pBoat->GetFrameHeight() * 5;

	m_Window.left = 0;
	m_Window.bottom = 0;
	m_Window.width = window.width;
	m_Window.height = window.height;

	m_pInputManager = inputManager;

	m_pSelect = pSoundManager->GetSoundEffectPointer("StartScreenSelect.wav");


}

StartScreen::~StartScreen()
{

	delete m_pStartScreenBackground;
	m_pStartScreenBackground = nullptr;

	delete m_pBoat;
	m_pBoat = nullptr;

	delete m_pText;
	m_pText = nullptr;

	delete m_pWater;
	m_pWater = nullptr;

	delete m_pLogo;
	m_pLogo = nullptr;

	delete m_pSelectData;
	m_pSelectData = nullptr;

	delete m_pFileSelect;
	m_pFileSelect = nullptr;

	delete m_pSword;
	m_pSword = nullptr;
}	


void StartScreen::Draw() const
{
	glPushMatrix();
	{
		switch (m_StartScreenState)
		{
		case StartScreen::StartScreenState::startscreen:
			m_pStartScreenBackground->Draw();
			m_pBoat->SetDrawPos(Point2f{ m_Boat.left,m_Boat.bottom });
			m_pBoat->Draw();
			m_pLogo->Draw();
			m_pWater->Draw();
			m_pText->Draw();
			break;
		case StartScreen::StartScreenState::fileSelect:
			m_pStartScreenBackground->Draw();
			m_pBoat->SetDrawPos(Point2f{ m_Boat.left,m_Boat.bottom });
			m_pBoat->Draw();
			m_pSelectData->Draw();
			m_pFileSelect->Draw();
			m_pSword->Draw();
			m_pWater->Draw();

			break;
		case StartScreen::StartScreenState::storyMode:
			break;
		case StartScreen::StartScreenState::options:
			break;
		default:
			break;
		}
	}
	glPopMatrix();
}

void StartScreen::Update(float elapsedSec)
{

	switch (m_StartScreenState)
	{
	case StartScreen::StartScreenState::startscreen:
		m_pStartScreenBackground->Update(elapsedSec);
		m_pBoat->Update(elapsedSec);
		m_pWater->Update(elapsedSec);
		UpdateBoatPos(elapsedSec);
		if (m_pInputManager->IsKeyPressed(SDL_SCANCODE_SPACE))
		{
			m_pSelect->Play(0);
			m_StartScreenState = StartScreenState::fileSelect;
		}

		break;
	case StartScreen::StartScreenState::fileSelect:
		m_pStartScreenBackground->SetCurrentFrame(1);
		m_pBoat->Update(elapsedSec);
		UpdateBoatPos(elapsedSec);
		if (m_pInputManager->IsKeyPressed(SDL_SCANCODE_SPACE))
		{
			m_pSelect->Play(0);
			m_isFinished = true;
		}

		break;
	case StartScreen::StartScreenState::storyMode:
		break;
	case StartScreen::StartScreenState::options:
		break;
	default:
		break;
	}
}

bool StartScreen::isFinished()
{
	return m_isFinished;
}

void StartScreen::UpdateBoatPos(float elapsedSec)
{
	if (m_Boat.bottom < m_Window.height * 0.05f && m_hitMaxHeight == false)
	{
		m_Boat.bottom += m_VerticalSpeed * elapsedSec;

	}
	else
	{
		m_hitMaxHeight = true;
	}
	if (m_hitMaxHeight)
	{
		m_Boat.bottom -= m_VerticalSpeed * elapsedSec;
		if (m_Boat.bottom <= -1)
		{
			m_hitMaxHeight = false;
		}
	}


}
