#pragma once
class TextureManager;
class InputManager;
class Sprite;
class SoundManager;
class SoundEffect;

class StartScreen final
{
public:

	StartScreen(TextureManager* spritemanager,InputManager* inputManager, const Window& window,SoundManager* pSoundManager);
	~StartScreen();

	enum class StartScreenState
	{
		startscreen,
		fileSelect,
		storyMode,
		options,
	};

	StartScreen(const StartScreen& other) = delete;
	StartScreen& operator=(const StartScreen& other) = delete;
	StartScreen(StartScreen&& other) = delete;
	StartScreen& operator=(StartScreen&& other) = delete;

	void Draw()const;
	void Update(float elapsedSec);
	bool isFinished();
private:
	void UpdateBoatPos(float elapsedSec);
	StartScreenState m_StartScreenState;

	Sprite* m_pStartScreenBackground;
	Sprite* m_pBoat;
	Sprite* m_pText;
	Sprite* m_pWater;
	Sprite* m_pLogo;
	Sprite* m_pSelectData;
	Sprite* m_pFileSelect;
	Sprite* m_pSword;

	const float m_VerticalSpeed;

	bool m_hitMaxHeight;
	bool m_isFinished;

	Rectf m_Window;

	Rectf m_Boat;
	Rectf m_Sword;
	InputManager* m_pInputManager;

	SoundEffect* m_pSelect;
};

