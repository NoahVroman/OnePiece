#pragma once
class Sprite;
class TextureManager;

class LoadingScreen final
{
public:
	LoadingScreen(TextureManager* spritemanager,const Window& window);

	LoadingScreen(const LoadingScreen& other) = delete;
	LoadingScreen& operator=(const LoadingScreen& other) = delete;
	LoadingScreen(LoadingScreen&& other) = delete;
	LoadingScreen& operator=(LoadingScreen&& other) = delete;

	~LoadingScreen();

	enum class CharacterType
	{
		luffy = 0,
		zoro = 1,
		nami = 2,
		usopp = 3,
	};

	void Draw()const;
	void Update(float elapsedSec);

	bool HasFinished();

private:

	void DrawBackground()const;
	void UpdateBackground(float elapsedsec);

	void DrawCharacters()const;
	void UpdateCharacters(float elapsedsec);
	void ResetPostions();


	Rectf m_LoadingScreenBackground;

	Rectf m_TopBlackBorder;
	Rectf m_BottomBlackBorder;
	Rectf m_Characters;
	Rectf m_CharacterNames;

	const float m_HorizontalSpeed;
	const float m_SlowTextSpeed;
	const float m_VerticalSpeed;

	bool m_HasFinishedBorders;
	bool m_isFinished;

	Rectf m_Window;

	Sprite* m_pLoadingScreenBackground;
	Sprite* m_pCharacters;
	Sprite* m_pCharacterNames;

	CharacterType m_CharacterType;
};

