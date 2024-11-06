#pragma once
#include <vector>
class Apple;
class TextureManager;
class Character;
class PickUpManager final
{
public:

	PickUpManager();
	~PickUpManager();

	PickUpManager(const PickUpManager& other) = delete;
	PickUpManager& operator=(const PickUpManager& rhs) = delete;
	PickUpManager(PickUpManager&& other) = delete;
	PickUpManager& operator=(PickUpManager&& rhs) = delete;

	void Draw()const;
	void Update(float elapsedSec);
	void AddPickup(const Point2f& pos, TextureManager* pTextureManager);
	bool IsOverlapping(const Rectf& shape);
	bool IsFinished();

	void ClearPickUpVector();

	const size_t& GetPickUpCount() const;

	Apple* GetPickup(const size_t& index);


private:

	std::vector<Apple*> m_pPickups;

	
};

