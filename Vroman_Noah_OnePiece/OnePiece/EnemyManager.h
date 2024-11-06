#pragma once
#include <vector>
#include "Vector2f.h"
class BaseEnemyClass;
class TextureManager;
class Level;
class Character;
class AxeMorgan;
class HUD;
class ProjectileManager;
class SoundManager;
class SoundEffect;

class EnemyManager final
{
 public:
     EnemyManager();
     ~EnemyManager();

     enum class Type
     {
         pirate,
         boss,
         bomber
     };


     EnemyManager(const EnemyManager& other) = delete;
     EnemyManager& operator=(const EnemyManager& rhs) = delete;
     EnemyManager(EnemyManager&& other) = delete;
     EnemyManager& operator=(EnemyManager&& rhs) = delete;


     void PushBackNewEnemy(const Type& typeOfEnemy, TextureManager* textureMananger, const Point2f& pos,ProjectileManager* pProjectileManager,SoundManager* pSoundManager);
     void UpdateAllEnemies(float elapsedSec, Level* level,Character* character, HUD* hud);
     void DrawAllEnemies()const;
     void ClearEnemies();

     void PopEnemies();

     Type GetTypeOfEnemy();

     const Rectf& GetShapeOfEnemy();

     AxeMorgan* GetAxeMorgan();

 private:
     std::vector<BaseEnemyClass*> m_pVectorOfEnemies;
     Type m_EnemyType;
};

