#pragma once
class TextureManager;
class Texture;

class Sprite final
{
public:
	Sprite(const std::string& filename, int nrCols, int nrRows, int framesPerSec, const Point2f& drawPos, float scale,TextureManager* pTextureManager);
	~Sprite();
	Sprite(const Sprite& other) = delete;
	Sprite& operator=(const Sprite& rhs) = delete;
	Sprite(Sprite&& other) = delete;
	Sprite& operator=(Sprite&& rhs) = delete;

	void Update(float elapsedSec);
	void Draw() const;
	float GetFrameWidth() const;
	float GetFrameHeight() const;

	void SetDrawPos(const Point2f& newPos);
	void SetCurrentFrame(int newframe);
	void SetFramerate(int newframerate);

	void HoldLastFrame();
	bool IsOnLastFrame();
	int GetCurrentFrame() const;
	int GetTotalFrames() const;


	bool IsPlayingBackwards() const;
	void SetPlayingBackwards(bool isPlayingBackwards);
	void PlayBackwards(float elapsedSec);

private:
	Texture* m_pTexture;
	std::string m_FileName;
	const int m_Columns;
	const int m_Rows;
	int m_FrameRate;
	Point2f m_DrawPos;

	const float m_Scale;

	int m_CurrentFrame;
	float m_AccumulatedSec;
	int m_TotalFrames;

	float m_FrameWidth;
	float m_FrameHeight;

	bool m_IsPlayingBackwards;

};

