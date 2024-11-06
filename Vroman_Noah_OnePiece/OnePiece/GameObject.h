#pragma once
#include <vector>
class Level;
class PickUpManager;
class GameObject 
{
public:
	GameObject(const Rectf& pos);
	virtual ~GameObject();

	GameObject(const GameObject& other) = delete;
	GameObject& operator=(const GameObject& rhs) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(GameObject&& rhs) = delete;


	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec,Level* level, PickUpManager* pPickUpManager) = 0;
	void IsHit(const Rectf& characterAttackShape);
	virtual bool IsFinished() =0;

	virtual const Rectf& GetObjectShape() const = 0;
	virtual const std::vector<Point2f>& GetPoleVertices() = 0;
	
protected:
	Rectf m_GameObjectPos;
	bool  m_isHit;

};

