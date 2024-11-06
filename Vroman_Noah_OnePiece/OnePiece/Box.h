#pragma once
#include "GameObject.h"
#include <Vector2f.h>
class TextureManager;
class Sprite;
class Level;
class PickUpManager;
class Apple;

class Box final : public GameObject
{
public:

	Box(const Rectf& pos, TextureManager* pTextureManager,PickUpManager* pPickUpManager);
	~Box();
	Box(const Box& other) = delete;
	Box& operator=(const Box& rhs) = delete;
	Box(Box&& other) = delete;
	Box& operator=(Box&& rhs) = delete;

	virtual void Draw()const override;
	virtual void Update(float elapsedSec,Level* level,PickUpManager* pPickUpManager)  override;
	virtual bool IsFinished()override;

	virtual const Rectf& GetObjectShape() const override;
	virtual const std::vector<Point2f>& GetPoleVertices() override { return std::vector<Point2f>{}; }


private:
	Rectf m_BoxRect;

	Vector2f m_BoxVelocity;
	const Vector2f m_BoxAcceleration;

	TextureManager* m_pTempTextureManager;

	PickUpManager* m_pPickUpManager;

	bool IsLoaded;

	Sprite* m_pBox;
	Sprite* m_pHitBox;
};

