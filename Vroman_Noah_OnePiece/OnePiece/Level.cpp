#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include "SVGParser.h"
#include "Character.h"
#include "Camera.h"
#include "TextureManager.h"
#include "EnemyManager.h"
#include "GameObjectMananger.h"
#include "PlatformManager.h"
#include "Pole.h"
#include "Platform.h"
#include "HUD.h"
#include "SoundManager.h"
#include "PickUpManager.h"
#include "ProjectileManager.h"
#include "Apple.h"
#include "AxeMorgan.h"
#include <SoundStream.h>
#include <SoundEffect.h>
#include <iostream>
#include "Spike.h"
#include "InputManager.h"

Level::Level(const Window& window, TextureManager* pTextureManager, SoundManager* pSoundManager,InputManager* pInputManager)
	:m_LevelBoundries{ 0,0,5113,1051 }
	, m_LevelState{ LevelState::level1}
	,m_LevelIndex{0}
	, isInLevel2{false}
	,m_FadeRect{Rectf{0,0,window.width,window.height}}
	,m_HitTriggerBox{false}
	,isInLevel3{false}
	,isInBossLevel{false}
	, m_HasPickedUp{ false }
	,m_hasHitBullet{false}
	,isBossDead{false}
	,m_CharacterDead{ false }
{
	m_Window = window;
	Initialise(window, pTextureManager,pSoundManager,pInputManager);
}

void Level::Initialise(const Window& window, TextureManager* pTextureManager,SoundManager* pSoundManager, InputManager* pInputManager)
{

	m_pCharacter = new Character{ Point2f{ 15,300 },pTextureManager,pSoundManager,pInputManager};
	m_pCamera = new Camera(window.width, window.height);
	m_pHUD = new HUD(window, pTextureManager);



	m_EnemyManager = new EnemyManager();
	m_pGameObjectManager = new GameObjectMananger();
	m_pPlatformManager = new PlatformManager();
	m_pPickUpManager = new PickUpManager();
	m_pProjectileManager = new ProjectileManager();
	m_pSoundManager = pSoundManager;

	m_LevelTriggerBox.width = 400;
	m_LevelTriggerBox.height = 400;
	m_LevelTriggerBox.bottom = 500;
	m_LevelTriggerBox.left = m_LevelBoundries.width;


	m_pMap1 = new Texture{ "Map1.png" };
	m_pMap2 = new Texture{ "Map2.png" };
	m_pMap3 = new Texture{ "Map3.png" };

	m_pBackgroudBoss = new Texture{ "BossBackground.png" };
	m_pMapBoss = new Texture{ "BossForeGround.png" };

	m_pLevelBackground = new Texture{ "LevelBackground.png" };
	m_pCamera->SetLevelBoundries(m_LevelBoundries);

	m_pSpike = new Spike(Rectf{ 0,0,0,0 });

	if (m_LevelState == LevelState::level1)
	{
		std::string path{ "Map1.svg" };
		SVGParser::GetVerticesFromSvgFile(path, m_MapVertices);

		//m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::box, Rectf{ 300,500,0,0 }, pTextureManager, 35, 1000,m_pPickUpManager);
		//m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::box, Rectf{ 1500,500,0,0 }, pTextureManager, 35, 1000, m_pPickUpManager);

        
		m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, pTextureManager, Point2f{ 4406,622 }, m_pProjectileManager, m_pSoundManager);
		m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, pTextureManager, Point2f{ 950,291 }, m_pProjectileManager, m_pSoundManager);
		m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, pTextureManager, Point2f{ 1150,358 }, m_pProjectileManager, m_pSoundManager);
		m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, pTextureManager, Point2f{ 4232,622 }, m_pProjectileManager, m_pSoundManager);

		m_pPlatformManager->PushBack(Rectf{ 3215,575,335 ,15 });
		m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::pole, Rectf{ 3780,200,30,1000 }, pTextureManager, 35, 1000, m_pPickUpManager);
	}

	m_FadeRectColor.r = 0;
	m_FadeRectColor.g = 0;
	m_FadeRectColor.b = 0;
	m_FadeRectColor.a = 0;

	m_pApplePickupEffect = pSoundManager->GetSoundEffectPointer("ApplePickup.wav");

}

void Level::FadeInOut(float elapsedSec)
{

	int levelIndex = static_cast<int>(m_LevelState); 
	if (utils::IsOverlapping(m_pCharacter->GetPlayerShape(), m_LevelTriggerBox))
	{
		m_HitTriggerBox = true;
		m_pCharacter->SetEnableInput(false);

	}
	if (m_HitTriggerBox)
	{
		if (m_FadeRectColor.a <= 1.f)
		{
			m_FadeRectColor.a += elapsedSec;
		}
	}
	else
	{
		if (m_FadeRectColor.a >= 0.0f)
		{
			m_FadeRectColor.a -= elapsedSec;
		}
	}

	if (m_FadeRectColor.a >= 1.f && m_HitTriggerBox == true)
	{
		++levelIndex;
		m_LevelState = static_cast<LevelState>(levelIndex);
		m_pCharacter->SetEnableInput(true);
		m_HitTriggerBox = false;
	}

}

void Level::InitialiseLevel2(TextureManager* spriteManager)
{
	ClearAllManagers();

	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::bomber, spriteManager, Point2f{ 4640,720 },m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, spriteManager, Point2f{ 1810,230 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, spriteManager, Point2f{ 2000,290 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, spriteManager, Point2f{ 2256,355 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::bomber, spriteManager, Point2f{ 3300,750 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, spriteManager, Point2f{ 3741,615 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, spriteManager, Point2f{ 4092,615 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, spriteManager, Point2f{ 3895,225 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, spriteManager, Point2f{ 4000,225 }, m_pProjectileManager, m_pSoundManager);
	m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::box, Rectf{ 4968,667,0,0 }, spriteManager, 35, 1000,m_pPickUpManager);




	m_pPlatformManager->PushBack(Rectf{ 3350,635,180 ,15 });
	m_pPlatformManager->PushBack(Rectf{ 3708,600,553 ,15 });
	m_pPlatformManager->PushBack(Rectf{ 4430,650,245 ,15 });
	m_pPlatformManager->PushBack(Rectf{ 4875,650,313 ,15 });

	m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::pole, Rectf{ 3439,650,33,1000 }, spriteManager, 35, 1000, m_pPickUpManager);
	m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::pole, Rectf{ 3708,580,553,15 }, spriteManager, 35, 1000, m_pPickUpManager);
	m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::pole, Rectf{ 4780,915,30,1000 }, spriteManager, 35, 1000, m_pPickUpManager);

	//m_pPlatformManager->PushBack(Rectf{ 3350,635,180 ,15 });

}

void Level::InitialiseLevel3(TextureManager* ptexturetManager)
{
	ClearAllManagers();

	m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::box, Rectf{ 4348,910,0,0 }, ptexturetManager, 35, 1000, m_pPickUpManager);
	m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::box, Rectf{ 1920,692,0,0 }, ptexturetManager, 35, 1000, m_pPickUpManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::bomber, ptexturetManager, Point2f{ 2720,650 }, m_pProjectileManager,m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::bomber, ptexturetManager, Point2f{ 2220,650 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, ptexturetManager, Point2f{ 3995,445 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, ptexturetManager, Point2f{ 3263,495 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, ptexturetManager, Point2f{ 1232,133}, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, ptexturetManager, Point2f{ 1676,133 }, m_pProjectileManager, m_pSoundManager);
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::pirate, ptexturetManager, Point2f{ 3434,190 }, m_pProjectileManager, m_pSoundManager);


	m_pPlatformManager->PushBack(Rectf{ 959,415,251 ,15 });
	m_pPlatformManager->PushBack(Rectf{ 1406,550,250 ,15 });
	m_pPlatformManager->PushBack(Rectf{ 1852,675,250 ,15 });
	m_pPlatformManager->PushBack(Rectf{ 3260,480,383 ,15 });
	m_pPlatformManager->PushBack(Rectf{ 3935,430,383 ,15 });
	m_pPlatformManager->PushBack(Rectf{ 4145,895,553 ,15 });
	m_pPlatformManager->PushBack(Rectf{ 4865,915,250 ,15 });

	m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::pole, Rectf{ 4145,880,553,15 }, ptexturetManager, 35, 1000, m_pPickUpManager);



	m_pSpike->SetShape(Rectf{ 2405,192,110,60 });

	m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::pole, Rectf{ 3935,415,383,15 }, ptexturetManager, 35, 1000, m_pPickUpManager);
	m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::pole, Rectf{ 2360,780,30,1000 }, ptexturetManager, 35, 1000, m_pPickUpManager);
	m_pGameObjectManager->PushBackNewObject(GameObjectMananger::TypeOfObject::pole, Rectf{ 2871,780,30,1000 }, ptexturetManager, 35, 1000, m_pPickUpManager);


}

void Level::InitialiseBossLevel(TextureManager* pTextureManger)
{
	ClearAllManagers();

	m_pSpike->SetShape(Rectf{ -200,-200,0,0 });

	m_pPlatformManager->PushBack(Rectf{ 0,-m_pMapBoss->GetHeight() * 2.f,1835.f,100 });
	m_EnemyManager->PushBackNewEnemy(EnemyManager::Type::boss, pTextureManger, Point2f{ 1500,100 }, m_pProjectileManager,m_pSoundManager);
}

void Level::ClearAllManagers()
{
	m_pPickUpManager->ClearPickUpVector();
	m_pProjectileManager->ClearProjectile();
	m_pPlatformManager->ClearPlatforms();
	m_EnemyManager->ClearEnemies();
	m_pGameObjectManager->ClearGameObjects();

}

Level::~Level()
{
	delete m_pMap1;
	m_pMap1 = nullptr;
	delete m_pMap2;
	m_pMap2 = nullptr;
	delete m_pMap3;
	m_pMap3 = nullptr;

	delete m_pCharacter;
	m_pCharacter = nullptr;

	delete m_pCamera;
	m_pCamera = nullptr;
	delete m_EnemyManager;
	m_EnemyManager = nullptr;

	delete m_pGameObjectManager;
	m_pGameObjectManager = nullptr;

	delete m_pPlatformManager;
	m_pPlatformManager = nullptr;

	delete m_pLevelBackground;
	m_pLevelBackground = nullptr;

	delete m_pHUD;
	m_pHUD = nullptr;

	delete m_pPickUpManager;
	m_pPickUpManager = nullptr;

	delete m_pBackgroudBoss;
	m_pBackgroudBoss = nullptr;

	delete m_pMapBoss;
	m_pMapBoss = nullptr;

	delete m_pProjectileManager;
	m_pProjectileManager = nullptr;

	delete m_pSpike;
	m_pSpike = nullptr;

}

void Level::HandleCollison(Rectf& shapeCharacter, Vector2f& velocityCharacter)
{
	m_pPlatformManager->HandleAllPlatformCollision(shapeCharacter, velocityCharacter);
	utils::HitInfo hitInfo2{};;


	utils::HitInfo HI{};
	const Point2f p1{ shapeCharacter.left + shapeCharacter.width *0.8f,shapeCharacter.bottom - 1.0f };
	const Point2f p2{ p1.x,p1.y + shapeCharacter.height / 2 };

	utils::HitInfo HIBOT{};

	const Point2f p5{ shapeCharacter.left + shapeCharacter.width *0.2f,shapeCharacter.bottom - 1.0f };
	const Point2f p6{ p1.x,p1.y + shapeCharacter.height / 2 };

	const Point2f p3{ shapeCharacter.left, shapeCharacter.bottom + shapeCharacter.height * 0.3f };
	const Point2f p4{ shapeCharacter.left + shapeCharacter.width, shapeCharacter.bottom + shapeCharacter.height * 0.3f };

	const Vector2f VectorCharacterRightBottom{ p2,p1 };


	const Vector2f VectorCharacter{ p3,p4 };

	const Vector2f VectorCharacterLefftBottom{ p5,p6 };

	const Vector2f VectorCharacterFlipped{ -VectorCharacter };

	if (m_LevelState != LevelState::boss)
	{

		if (utils::Raycast(m_MapVertices[m_LevelIndex], p3, p4, hitInfo2))
		{
			const float DotProduct = VectorCharacter.DotProduct(hitInfo2.normal);

			const float DotProductFlipped = VectorCharacterFlipped.DotProduct(hitInfo2.normal);

			if (DotProduct < 0)
			{
				shapeCharacter.left = hitInfo2.intersectPoint.x - shapeCharacter.width;
				velocityCharacter.x = 0;
			}
			if (DotProductFlipped < 0)
			{
				shapeCharacter.left = hitInfo2.intersectPoint.x;
				velocityCharacter.x = 0;
			}
		}
		else if (utils::Raycast(m_MapVertices[m_LevelIndex], p1, p2, HI))
		{
			if (utils::Raycast(m_MapVertices[m_LevelIndex], p5, p6, HIBOT))
			{
				const float DotProduct = VectorCharacterLefftBottom.DotProduct(HIBOT.normal);
				if (DotProduct < 0)
				{
					velocityCharacter.y = 0.f;
					shapeCharacter.bottom = HI.intersectPoint.y;

				}
			}

			const float DotProduct = VectorCharacterRightBottom.DotProduct(HI.normal);
			if (DotProduct < 0)
			{
				velocityCharacter.y = 0.f;
				shapeCharacter.bottom = HI.intersectPoint.y;

			}
		}

	}
	if (shapeCharacter.left < m_LevelBoundries.left)
	{
		velocityCharacter.x = 0;
		shapeCharacter.left = m_LevelBoundries.left;
	}
	if (shapeCharacter.left > m_LevelBoundries.left + m_LevelBoundries.width - shapeCharacter.width)
	{
		velocityCharacter.x = 0;
		shapeCharacter.left = m_LevelBoundries.left + m_LevelBoundries.width - shapeCharacter.width;
	}

}

bool Level::isOnGround(const Rectf& ShapeAvatar, const Vector2f& VelocityAvatar) const
{
	utils::HitInfo HI{};
	Point2f p1{ ShapeAvatar.left + ShapeAvatar.width *0.8f,ShapeAvatar.bottom - 1.0f };
	Point2f p2{ p1.x,p1.y + ShapeAvatar.height / 2 };

	utils::HitInfo HIBOT{};

	Point2f p5{ ShapeAvatar.left + ShapeAvatar.width * 0.2f,ShapeAvatar.bottom - 1.0f };
	Point2f p6{ p1.x,p1.y + ShapeAvatar.height / 2 };


	if (m_LevelState != LevelState::boss)
	{
		if (utils::Raycast(m_MapVertices[m_LevelIndex], p5, p6, HI))
		{
			return true;
		}
		else if (utils::Raycast(m_MapVertices[m_LevelIndex], p1, p2, HIBOT))
		{
			return true;
		}


	}
	if (m_pPlatformManager->isPlayerOnGround(ShapeAvatar, VelocityAvatar))
	{
		return true;
	}

	return false;


}

void Level::DrawLevel()const
{
	utils::SetColor(Color4f{0,1,1,1});
	utils::DrawRect(m_LevelTriggerBox);

	glPushMatrix();
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (m_LevelState != LevelState::boss)
			{
				m_pLevelBackground->Draw(Rectf{ -m_pCamera->GetCameraTransform(m_pCharacter->GetPlayerShape()).x * 0.2f + (i * m_pLevelBackground->GetWidth() * 8),0,m_pLevelBackground->GetWidth() * 8,m_pLevelBackground->GetHeight() * 8 });
			}

		}
		if (m_LevelState == LevelState::boss)
		{
			m_pBackgroudBoss->Draw(Rectf{0,0,m_pBackgroudBoss->GetWidth()*6.f,m_pBackgroudBoss->GetHeight()*6.f});
		}
	}
	glPopMatrix();

	glPushMatrix();
	{

		m_pCamera->Transform(m_pCharacter->GetPlayerShape());

		if (m_LevelState == LevelState::boss)
		{
			m_pMapBoss->Draw(Rectf{0,-m_pMapBoss->GetHeight()*2.f,m_pMapBoss->GetWidth() * 5.f,m_pMapBoss->GetHeight() * 5.f});
		}
		if (m_LevelState == LevelState::level1)
		{
			m_pMap1->Draw();

		}
		if (m_LevelState == LevelState::level2)
		{
			m_pMap2->Draw();
		}
		if (m_LevelState == LevelState::level3) 
		{
			m_pMap3->Draw();
		}

		m_EnemyManager->DrawAllEnemies();
		m_pCharacter->Draw();
		m_pPickUpManager->Draw();
		m_pPickUpManager->Draw();
		m_pGameObjectManager->DrawObjects();
		m_pProjectileManager->Draw();
	}
	glPopMatrix();

	
	m_pHUD->Draw();

	if (m_LevelState == LevelState::boss)
	{
		m_pHUD->DrawBossHUD();
	}


	utils::SetColor(m_FadeRectColor);
	utils::FillRect(m_FadeRect);

}

void Level::DrawBackground() const
{
	glPushMatrix();
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (m_LevelState != LevelState::boss)
			{
				m_pLevelBackground->Draw(Rectf{ -m_pCamera->GetCameraTransform(m_pCharacter->GetPlayerShape()).x * 0.2f + (i * m_pLevelBackground->GetWidth() * 8),0,m_pLevelBackground->GetWidth() * 8,m_pLevelBackground->GetHeight() * 8 });
			}

		}
		if (m_LevelState == LevelState::boss)
		{
			m_pBackgroudBoss->Draw(Rectf{ 0,0,m_pBackgroudBoss->GetWidth() * 6.f,m_pBackgroudBoss->GetHeight() * 6.f });
		}
	}
	glPopMatrix();

	glPushMatrix();
	{

		m_pCamera->Transform(m_pCharacter->GetPlayerShape());
		if (m_LevelState == LevelState::boss)
		{
			m_pMapBoss->Draw(Rectf{ 0,-m_pMapBoss->GetHeight() * 2.f,m_pMapBoss->GetWidth() * 5.f,m_pMapBoss->GetHeight() * 5.f });
		}
		if (m_LevelState == LevelState::level1)
		{
			m_pMap1->Draw();

		}
		if (m_LevelState == LevelState::level2)
		{
			m_pMap2->Draw();
		}
		if (m_LevelState == LevelState::level3)
		{
			m_pMap3->Draw();
		}
	}
	glPopMatrix();

}

void Level::UpdateLevel(float elaspedsec, TextureManager* spriteManager)
{
	m_pHUD->Update(elaspedsec, m_pCharacter);
	FadeInOut(elaspedsec);

	switch (m_LevelState)
	{
	case Level::LevelState::level1:
		isInLevel2 = false;
		isInLevel3 = false;
		break;
	case Level::LevelState::level2:
		isInLevel3 = false;
		if (!isInLevel2)
		{
			InitialiseLevel2(spriteManager);
			m_LevelBoundries.width = 5705;
			m_LevelBoundries.height = 1072;
			m_pCamera->SetLevelBoundries(m_LevelBoundries);
			m_LevelTriggerBox.left = m_LevelBoundries.width - 25;
			m_LevelIndex = 1;
			std::string pathMap2{ "Map2.svg" };
			SVGParser::GetVerticesFromSvgFile(pathMap2, m_MapVertices);
			m_pCharacter->SetPosition(Point2f{ 15,250 });
			isInLevel2 = true;
		}
		break;

	case Level::LevelState::level3:
		if (!isInLevel3)
		{
			InitialiseLevel3(spriteManager);
			m_LevelBoundries.width = 5056;
			m_LevelBoundries.height = 1188;
			m_pCamera->SetLevelBoundries(m_LevelBoundries);
			m_LevelTriggerBox.left = m_LevelBoundries.width -25;
			m_LevelTriggerBox.bottom = 552.40f;
			m_LevelIndex = 2;
			std::string pathMap3{ "Map3.svg" };
			SVGParser::GetVerticesFromSvgFile(pathMap3, m_MapVertices);
			m_pCharacter->SetPosition(Point2f{ 15,250 });
			isInLevel3 = true;
		}
		break;
	case Level::LevelState::boss:
		if (!isInBossLevel)
		{
			InitialiseBossLevel(spriteManager);
			m_LevelBoundries.width = 1835.f;
			m_LevelBoundries.height = 640.f;
			m_pCamera->SetLevelBoundries(m_LevelBoundries);
			//m_LevelIndex = 1;
			m_pCharacter->SetPosition(Point2f{ 5,100 });
			isInBossLevel = true;
		}
		break;

	}

	static_cast<Character*>(m_pCharacter)->HandleGrabbing(elaspedsec, m_pGameObjectManager);

	if (m_pHUD->GetBossDead())
	{
		isBossDead = true;
	}

	if (m_pHUD->GetPlayerDead())
	{
		m_CharacterDead = true;
	}

	m_EnemyManager->UpdateAllEnemies(elaspedsec, this, m_pCharacter,m_pHUD);
	m_pCharacter->Update(elaspedsec, this, m_pCharacter->GetPlayerShape());
	m_pGameObjectManager->UpdateObjects(elaspedsec, this, m_pPickUpManager);
	m_pGameObjectManager->HitBox(m_pCharacter);
	m_pPickUpManager->Update(elaspedsec);
	m_pProjectileManager->Update(elaspedsec,this,m_pCharacter->GetPlayerShape());

	if (m_pProjectileManager->GetProjectileShape().left < m_LevelBoundries.left)
	{
		m_hasHitBullet = false;
		m_pProjectileManager->PopProjectile();
	}
	else if (m_pProjectileManager->GetProjectileShape().left > (m_LevelBoundries.left + m_LevelBoundries.width))
	{
		m_hasHitBullet = false;
		m_pProjectileManager->PopProjectile();
	}

	if (m_pProjectileManager->GetFinished())
	{
		m_hasHitBullet = false;
		m_pProjectileManager->PopProjectile();
	}

	if (m_pProjectileManager->IsOverlapping(m_pCharacter->GetPlayerShape()) && m_hasHitBullet == false)
	{
		m_hasHitBullet = true;
		m_pCharacter->IsHit(m_pProjectileManager->GetProjectileShape().left, float(m_pProjectileManager->GetDamage()), elaspedsec);

	}

	if (m_pSpike->IsOverlapping(m_pCharacter->GetPlayerShape()))
	{
		m_pCharacter->IsHit(m_pSpike->GetShape().left, float(m_pSpike->GetDamage()), elaspedsec);
	}

	
	if (m_pPickUpManager->IsOverlapping(m_pCharacter->GetPlayerShape()))
	{
		bool isAnyPickupOverlapping = false; 

		for (size_t i = 0; i < m_pPickUpManager->GetPickUpCount(); i++)
		{
			Apple* pickup = m_pPickUpManager->GetPickup(i);

			if (pickup->IsOverlapping(m_pCharacter->GetPlayerShape()))
			{
				if (!pickup->GetCollected()) 
				{
					m_pCharacter->AddHealth(3);
					pickup->SetCollected(true); 
				}

				isAnyPickupOverlapping = true;
			}
		}

		if (isAnyPickupOverlapping)
		{
			m_pApplePickupEffect->Play(0);
			m_pCharacter->SetHealthIncreased(true); 
		}
	}
}

bool Level::GetBossDead() const
{
	return isBossDead;
}

bool Level::GetCharacterDead() const
{
	return m_CharacterDead;
}

void Level::PressKeyDown(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.scancode)
	{

	}
}

void Level::PressKeyUp(const SDL_KeyboardEvent& e)
{
	//m_pCharacter->PressKeyUp(e);
}

