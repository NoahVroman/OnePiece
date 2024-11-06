#pragma once
#include "GameObject.h"
#include <vector>

class Level;
class PickUpManager;
class Pole final : public GameObject
{
public:

	Pole(const Rectf& pos);
	~Pole();

	Pole(const Pole& other) = delete;
	Pole& operator=(const Pole& rhs) = delete;
	Pole(Pole&& other) = delete;
	Pole& operator=(Pole&& rhs) = delete;

	virtual void Draw() const override{};
	virtual void Update(float elapsedSec,Level* level, PickUpManager* pPickUpManager)override {};
	virtual const Rectf& GetObjectShape() const override;
	virtual const std::vector<Point2f>& GetPoleVertices() override;
	virtual bool IsFinished() override { return false; };


private:

	std::vector<Point2f> m_Vertices;
	Rectf m_PoleRect;

};

