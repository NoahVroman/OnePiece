#pragma once
class TextureManager;
class Sprite;

class Apple final
{
public:
	Apple(const Point2f& pos,TextureManager* pTextureManager);
	~Apple();

	Apple(const Apple& other) = delete;
	Apple& operator=(const Apple& rhs) = delete;
	Apple(Apple&& other) = delete;
	Apple& operator=(Apple&& rhs) = delete;
	
	void Draw()const;
	void Update(float elapsedSec);

	bool IsOverlapping(const Rectf& shape);

	bool IsFinished();
	void SetCollected(bool collected);
	bool GetCollected()const;
	const Rectf& GetShape()const;

private:
	Rectf m_Shape;
	Sprite* m_pApple;
	bool m_isHit;
	Point2f m_EndPoint;
	bool isFinished;
	bool m_IsCollected;

	float m_DeathTimer;
	float m_MaxDeathTimer;
	bool m_End;
	const float m_OriginalDeathTime;


};

