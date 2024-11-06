#pragma once
class TextureManager;
class InputManager;
class SoundManager;
class SoundEffect;
class Sprite;

class PauseScreen final
{
public:
	PauseScreen(TextureManager* spritemanager, InputManager* inputManager,SoundManager* soundManager, const Window& window);
	~PauseScreen();

	enum class PauseState
	{
		Continue,
		EndGame,
	};

	PauseScreen(const PauseScreen& other) = delete;
	PauseScreen& operator=(const PauseScreen& other) = delete;
	PauseScreen(PauseScreen&& other) = delete;
	PauseScreen& operator=(PauseScreen&& other) = delete;

	void Draw()const;
	void Update(float elapsedSec);

	bool IsGameEnded()const;
	bool Continue()const;
	
	void SetContinue(bool Conitune);


private:

	Rectf m_Window;
	bool m_EndGame;
	bool m_Continue;
	SoundEffect* m_pSelectSound;

	Sprite* m_pPauseScreen;

	InputManager* m_pInputManager;

};

