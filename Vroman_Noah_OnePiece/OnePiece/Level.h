#pragma once
#include "utils.h"
#include <vector>
struct Vector2f;
class Texture;
class TextureManager;
class Character;
class Camera;
class EnemyManager;
class Pole;
class GameObjectMananger;
class Platform;
class PlatformManager;
class HUD;
class SoundManager;
class SoundEffect;
class PickUpManager;
class Apple;
class AxeMorgan;
class ProjectileManager;
class Spike;
class InputManager;
class Level final
{
public:
	Level(const Window& window, TextureManager* spriteManager, SoundManager* pSoundManager, InputManager* pInputManager);
	~Level();

	Level(const Level& other) = delete;
	Level& operator=(const Level& rhs) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& rhs) = delete;


	enum class LevelState
	{
		level1,
		level2,
		level3,
		boss,
	};

	void HandleCollison(Rectf& shapeCharacter, Vector2f& velocityCharacter);
	bool isOnGround(const Rectf& ShapeAvatar, const Vector2f& VelocityAvatar) const;

	void DrawLevel() const;
	void DrawBackground()const;

	void UpdateLevel(float elaspedsec, TextureManager* spriteManager);

	bool GetBossDead()const;

	bool GetCharacterDead()const;

	void PressKeyDown(const SDL_KeyboardEvent& e);
	void PressKeyUp(const SDL_KeyboardEvent& e);




private:
	std::vector<std::vector<Point2f>> m_MapVertices;
	
	int m_LevelIndex;

	LevelState m_LevelState;

	//Boudries
	Rectf m_LevelBoundries;
	Rectf m_LevelTriggerBox;
	Color4f m_FadeRectColor;
	const Rectf m_FadeRect;

	EnemyManager* m_EnemyManager;
	GameObjectMananger* m_pGameObjectManager;
	PlatformManager* m_pPlatformManager;
	PickUpManager* m_pPickUpManager;
	ProjectileManager* m_pProjectileManager;
	SoundManager* m_pSoundManager;

	Window m_Window;
	Spike* m_pSpike;

	//Textures and Sprites
	const Texture* m_pMap1;
	const Texture* m_pMap2;
	const Texture* m_pMap3;
	const Texture* m_pLevelBackground;

	const Texture* m_pMapBoss;
	const Texture* m_pBackgroudBoss;
	//Object Pointers
	Character* m_pCharacter;
	Camera* m_pCamera;
	HUD* m_pHUD;

	SoundEffect* m_pApplePickupEffect;

	bool isInLevel2;
	bool isInLevel3;
	bool isInBossLevel;
	bool m_HitTriggerBox;
	bool m_HasPickedUp;
	bool m_hasHitBullet;
	bool isBossDead;
	bool m_CharacterDead;
	//Private Functions
	void Initialise(const Window& window, TextureManager* spriteManager, SoundManager* pSoundManager, InputManager* pInputManager);
	
	void FadeInOut(float elapsedSec);

	void InitialiseLevel2(TextureManager* spriteManager);

	void InitialiseLevel3(TextureManager* ptexturetManager);
	
	void InitialiseBossLevel(TextureManager* pTextureManger);

	void ClearAllManagers();

};

