#pragma once
#include "Vector2f.h"
#include <chrono>
#include <vector>
class Level;
class Punch;
class TextureManager;
class InputManager;
class Sprite;
class SoundEffect;
class SoundManager;
class GameObjectMananger;
class InputManager;

class Character final
{
public:
	Character(const Point2f& characterPos,TextureManager* pTextureManager,SoundManager* pSoundManager, InputManager* pInputManager);
	~Character();

	Character(const Character& other) = delete;
	Character& operator=(const Character& rhs) = delete;
	Character(Character&& other) = delete;
	Character& operator=(Character&& rhs) = delete;

	enum class CharacterActionState
	{
		alive,
		idle,
		walking,
		sprinting,
		jumping,
		falling,
		singlePunch,
		doublePunch,
		triplePunch,
		jumpAttack,
		dead,
		horizontalGrabbing,
		horizontalGrabbed,
		verticalGrabbing,
		verticalGrabbed,
		diagionalGrabbing,
		diagonalGrabbed,
		flying,
		rolling,
		startTransition,
		damaged,
		
	};

	void Draw() const;
	void Update(float elapsedSec, Level* level, const Rectf& characterPos);

	const Rectf& GetPlayerShape()const;
	const Rectf& GetAttackShape()const;
	void SetPosition(const Point2f& pos);
	void SetEnableInput(bool EnableInput);

	void IsHit(float pointOfContact, float damage,float elapsedSec);
	void AddHealth(float amount);
	float GetDamage()const;

	float GetHealth()const;
	float GetMaxHealth()const;

	bool IsHealthIncreased() const;
	void SetHealthIncreased(bool value);

	bool GetIsHit()const;
	const Vector2f& GetCharacterVelocity() const;

	bool GetIsLookingRight() const;
	void HandleGrabbing(float elapsedSec, GameObjectMananger* gameObjectManager);

	void SetActionSate(const CharacterActionState& state);
	CharacterActionState GetSate() const;
	void ResetActionState();
	

private:

	//////////////////////////
	/// PRIVATE  VARIABLES///
	//////////////////////////

	void HandleMovement(float elapsedSec, Level* level);
	void HandleGrabbing(const Uint8* states);
	void HandleGatling(bool isOnGround,float elapsedSec);

	void DrawSprites() const;
	void UpdateSprites(float elapsedSec);
	void ResetAnimationFrames();

	void UpdateHitboxes(float width,float height,float scale);
	void UpdateCharacterOnGround(const Uint8* keyboardState, float elapsedSec);
	void UpdateCharacterInAir(float elapsedSeconds);
	void UpdateMovement(const Uint8* keyboardState, Level* level);
	void UpdateCharacterActionState(bool isOnGround);

	void HandleAttacks(float elapsedSec);

	void SetInvincible();


	int	  m_NumberOfClicks;

	//////////////////////////
	/// CHARACTER VARIABLES///
	//////////////////////////

	Rectf				 m_CharacterShape;
	Vector2f			 m_CharacterVel;
	Vector2f			 m_CharacterAccel;
						 
	////////////////////////
	///		 BOOLS		 ///
	////////////////////////

	bool				 m_EnableInput;
	bool				 m_isSpacebarPressedOnce;
	bool				 m_LookingRight;
	bool			     m_Walking;
	bool				 m_IsHit;
	bool                 m_Invincible;
	bool				 m_IsHealthIncrease;
	
	bool				 m_HasClickedOnce;
	bool				 m_HasClickedTwice;
	bool				 m_HasClickedThree;
	
	bool				 m_HasEnterdJumpingAttack;

	////////////////////////
	///		MOVEMENT     ///
	////////////////////////		

	const float			 m_HorizontalSpeed;

	const float			 m_JumpSpeed;
	
	const float			 m_DashHorizontalSpeed;
	const float			 m_DashVerticalSpeed;

	

	////////////////////////
	///		POINTERS     ///
	///////////////////////

	Sprite*				 m_pCharacterIdle;
	Sprite*				 m_pCharacterRunning;
	Sprite*				 m_pCharacterJumping;
	Sprite*				 m_pCharacterFalling;
	Sprite*				 m_pSinglePunchAnimation;
	Sprite*				 m_pCharacterDoublePunch;
	Sprite*			     m_pCharacterTripleAttack;
	Sprite*			     m_pCharacterGrabArm;
	Sprite*			     m_pCharacterGrabVertical;
	Sprite*		         m_pCharacterGrabbedVertical;
	Sprite*			     m_pCharacterGrabDiagonal;
	Sprite*				 m_pCharacterGrabbedHorizontal;
	Sprite*			     m_pCharacterGrabbedDiagonal;
	Sprite*			     m_pCharacterDead;
	Sprite*				 m_pCharacterDamaged;
	Sprite*				 m_pFlying;
	Sprite*		         m_pRolling;
	Sprite*			     m_pJumpAttack;

	SoundEffect*		 m_pPunchEffect;
	SoundEffect*	     m_pGrabEffect;
	SoundEffect*		 m_pJumpEffect;
	SoundEffect*		 m_pDeathSound;


	Rectf				 m_AttackRect;
	Point2f				 m_HandPoint;
	Point2f				 m_MaxReachPoint;

	CharacterActionState m_CharacterActionState;


	InputManager* m_pInputManager;

	//////////////////////
	//		HEALTH      //
	/////////////////////

	float m_CurrentHealth;
	float m_MaxHealth;
	float m_Damage;

	float m_Timer;
	const float m_OriginalMaximumInvincibleTimer;
	float m_MaxInvincibiltyTimer;
	



	float m_FlyTimer;
	float m_MaxFlyTime;
	bool  m_Flying;
	const float m_OriginalFlyTime;


};



