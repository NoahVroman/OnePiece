#pragma once
class TextureManager;
class Sprite;

class EndScreen final
{
public:

	EndScreen(TextureManager* pTextureManager);
	~EndScreen();

	EndScreen(const EndScreen& other) = delete;
	EndScreen& operator=(const EndScreen& rhs) = delete;
	EndScreen(EndScreen&& other) = delete;
	EndScreen& operator=(EndScreen&& rhs) = delete;

	void Draw()const;
	void Update(float elapsedSec);

private:

	Sprite* m_pEndscreen;

};

