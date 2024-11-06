#pragma once
#include "../Engine/BaseGame.h"
class Level;
class TextureManager;
class LoadingScreen;
class StartScreen;
class PauseScreen;
class InputManager;
class SoundManager;
class SoundStream;
class SoundEffect;
class EndScreen;
class DeathScreen;
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );

	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;

	~Game();

	enum class GameState
	{
		loadingScreen,
		startScreen,
		play,
		pauseScreen,
		endScreen,
		deathScreen

	};


	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	// DATA MEMBERS
	const Window m_Window;
	Level* m_pLevel;

	TextureManager* m_pTextureManager;
	InputManager* m_pInputManager;
	SoundManager* m_pSoundManager;


	SoundEffect* m_pPauseScreenEffect;

	SoundStream* m_pIntroScreenMusic;
	SoundStream* m_pGameMusic;
	SoundStream* m_pEndScreenMusic;

	LoadingScreen* m_pLoadingScreen;
	StartScreen* m_pStartScreen;
	PauseScreen* m_pPauseScreen;
	EndScreen* m_pEndScreen;
	DeathScreen* m_pDeathScreen;

	bool m_IsPlaying;
	bool m_IsInPause;
	bool m_IsInEndScreen;
	bool m_HasHitMaxAlpha;
	bool m_Restart;

	bool m_Continue;

	GameState m_GameState;

	Color4f m_FadeColor;
	Rectf m_FadeRect;


	// FUNCTIONS
	void Initialize(const Window& window);
	void Cleanup( );
	void ClearBackground( ) const;

	void FadeInOut(float elapsedSec);
	void RestartLevel(const Window& window);

};