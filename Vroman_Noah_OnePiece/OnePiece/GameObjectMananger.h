#pragma once
#include <vector>
class Pole;
class TextureManager;
class GameObject;
class Character;
class Level;
class PickUpManager;
class GameObjectMananger final
{
public:
	GameObjectMananger();
	~GameObjectMananger();

	GameObjectMananger(const GameObjectMananger& other) = delete;
	GameObjectMananger& operator=(const GameObjectMananger& rhs) = delete;
	GameObjectMananger(GameObjectMananger&& other) = delete;
	GameObjectMananger& operator=(GameObjectMananger&& rhs) = delete;

	enum class TypeOfObject
	{
		pole,
		box
	};

	void PushBackNewObject(TypeOfObject type,const Rectf& pos, TextureManager* spritemananger, float width, float height, PickUpManager* pPickUpManager);
	void UpdateObjects(float elapsedSec,Level* level, PickUpManager* pPickUpManager);
	void DrawObjects()const;
	void ClearGameObjects();
	const Rectf& GetObjectShape() const;

	bool isFinished();

	void HitBox(Character* character);

	
	bool GetCanBeGrabbed();
	const std::vector<GameObject*>& GetObjectVector();

	const std::vector<Point2f>& GetPoleVertices();



private:

	std::vector<GameObject*> m_pVectorOfGameObjects;
	TypeOfObject m_TypeOfObject;
	bool m_CanBeGrabbed;
	

};

