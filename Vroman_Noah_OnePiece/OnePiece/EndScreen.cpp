#include "pch.h"
#include "EndScreen.h"
#include "TextureManager.h"
#include "Sprite.h"
EndScreen::EndScreen(TextureManager* pTextureManager)
{
	m_pEndscreen = new Sprite("EndScreen.png", 8, 5, 10, Point2f{ -100,0 }, 6.1f, pTextureManager);
}

EndScreen::~EndScreen()
{
	delete m_pEndscreen;
	m_pEndscreen = nullptr;
}

void EndScreen::Draw() const
{
	m_pEndscreen->Draw();
}

void EndScreen::Update(float elapsedSec)
{
	if (!m_pEndscreen->IsOnLastFrame())
	{
		m_pEndscreen->Update(elapsedSec);
	}
}
