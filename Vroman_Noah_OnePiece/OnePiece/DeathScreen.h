#pragma once
class Sprite;
class TextureManager;
class InputManager;
class SoundManager;

class DeathScreen final
{
public:

	DeathScreen(TextureManager* spritemanager, InputManager* inputManager, SoundManager* soundManager, const Window& window);
	~DeathScreen();

	DeathScreen(const DeathScreen& other) = delete;
	DeathScreen& operator=(const DeathScreen& rhs) = delete;
	DeathScreen(DeathScreen&& other) = delete;
	DeathScreen& operator=(DeathScreen&& rhs) = delete;

	void Draw()const;
	void Update(float elapsedSec);

	bool Restart()const;
	bool Quit()const;

private:
	

	bool m_Restart;
	bool m_Quit;

	InputManager* m_pInputmanager;
	Sprite* m_pDeathScreen;
	const Window& m_Window;

};

