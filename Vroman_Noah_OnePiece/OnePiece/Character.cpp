#include "pch.h"
#include "Character.h"
#include "utils.h"
#include "Level.h"
#include "TextureManager.h"
#include "Sprite.h"
#include <chrono>
#include <iostream>
#include "InputManager.h"
#include "GameObjectMananger.h"
#include "GameObject.h"
#include "SoundManager.h"
#include <SoundEffect.h>
#include "InputManager.h"
using namespace std;
Character::Character(const Point2f& characterPos, TextureManager* pTextureManager, SoundManager* pSoundManager,InputManager* pInputManager)
	:m_CharacterVel{}
	, m_CharacterAccel{ 0,-2000 }
	, m_HorizontalSpeed{ 400.0f }
	, m_EnableInput{ true }
	, m_JumpSpeed{ 900.0f }
	, m_CharacterActionState{ CharacterActionState::startTransition }
	, m_isSpacebarPressedOnce{ false }
	, m_LookingRight{ true }
	, m_DashHorizontalSpeed{2500.0f}
	, m_DashVerticalSpeed{1500.f}
	, m_Walking{false}
	, m_CurrentHealth{10.0f}
	, m_MaxHealth{10.0f}
    , m_Damage{ 1.f }
	, m_IsHit{ false }
	, m_Timer{0.0f}
	, m_MaxInvincibiltyTimer{ 2.f }
	, m_Invincible{ false }
    , m_OriginalMaximumInvincibleTimer{2.f}
	, m_IsHealthIncrease{ false }
	, m_NumberOfClicks{0}
	, m_HasClickedOnce{false}
	, m_HasClickedTwice{ false }
	, m_HasClickedThree{ false }
	, m_FlyTimer{ 0.0f }
	, m_MaxFlyTime{ 0.25f }
	, m_Flying{ false }
	, m_OriginalFlyTime{ 0.25f }
	, m_HasEnterdJumpingAttack{ false }

	
{
	m_CharacterShape.left = characterPos.x;
	m_CharacterShape.bottom = characterPos.y;
	m_CharacterShape.width = 100;
	m_CharacterShape.height = 200;

	m_HandPoint.x = m_CharacterShape.left + m_CharacterShape.width;
	m_HandPoint.y = m_CharacterShape.bottom + m_CharacterShape.height/2.f;

	m_MaxReachPoint.x = 0.0f;
	m_MaxReachPoint.y = 0.0f;


	m_AttackRect.width = 75;
	m_AttackRect.height = 40;
	
	const std::string CharacterIdle			     { "CharacterIdle.png" };
	const std::string CharacterRunning		     { "CharacterRunning.png" };
	const std::string CharacterJumping		     { "CharacterJumping.png" };
	const std::string CharacterFalling		     { "CharacterFalling.png" };
	const std::string CharacterSinglePunch	     { "CharacterSinglePunch.png" };
	const std::string CharacterDoublePunch	     { "CharacterDoublePunch.png" };
	const std::string CharacterTripleAttack		 { "CharacterGatling.png" };
	const std::string CharacterGrab			     { "CharacterGrab.png" };
	const std::string CharacterVerticalGrab	     { "CharacterGrabVertical.png" };
	const std::string CharacterVerticalGrabbed   { "CharacterVerticalGrabbed.png" };
	const std::string CharacterDiagonalGrab	     { "CharacterGrabDiagonal.png" };
	const std::string CharacterHorziontalGrabbed { "CharacterGrabbed.png" };
	const std::string CharacterDiagonalGrabbed   { "CharacterGrabbedDiagonal.png" };
	const std::string CharacterDead			     { "CharacterDie.png" };
	const std::string CharacterDamaged           { "Damaged.png"};
	const std::string Flying					 { "Flying.png" };
	const std::string Rolling					 { "Rolling.png" };
	const std::string JumpAttack				 { "JumpAttack.png" };
	const float scale{ 4 };


	m_pCharacterIdle			  =		new Sprite(CharacterIdle, 7, 1, 7,					 Point2f{ 0,0 }, scale,pTextureManager);
	m_pCharacterRunning			  =		new Sprite(CharacterRunning, 6, 1, 8,				 Point2f{ 0,0 }, scale,pTextureManager);
	m_pCharacterJumping			  =		new Sprite(CharacterJumping, 5, 1, 10,				 Point2f{ 0,0 }, scale,pTextureManager);
	m_pCharacterFalling			  =		new Sprite(CharacterFalling, 6, 1, 10,				 Point2f{ 0,0 }, scale,pTextureManager);
	m_pSinglePunchAnimation		  =	    new Sprite(CharacterSinglePunch, 4, 1, 9,			 Point2f{ 0,0 }, scale,pTextureManager);
	m_pCharacterDoublePunch		  =	    new Sprite(CharacterDoublePunch, 7, 1, 15,			 Point2f{ 0,0 }, scale,pTextureManager);
	m_pCharacterTripleAttack      =		new Sprite(CharacterTripleAttack, 11, 1, 15,		 Point2f{ 0,0 }, scale,pTextureManager);
	m_pCharacterGrabArm		      =	    new Sprite(CharacterGrab, 17, 1, 30,				 Point2f{ 0,0 }, scale,pTextureManager);
	m_pCharacterGrabVertical	  =		new Sprite(CharacterVerticalGrab, 15, 1, 17,		 Point2f{ 0,0 }, scale,pTextureManager);
	m_pCharacterGrabbedVertical   =	    new Sprite(CharacterVerticalGrabbed, 7, 1, 30, Point2f{ 0,0 }, scale, pTextureManager);
	m_pJumpAttack                 =		new Sprite(JumpAttack, 11, 1, 20, Point2f{ 0,0 }, scale, pTextureManager);


	m_pCharacterGrabDiagonal	  =		new Sprite(CharacterDiagonalGrab, 15, 1, 17,		 Point2f{ 0,0 }, scale,pTextureManager);
	m_pCharacterGrabbedHorizontal =		new Sprite(CharacterHorziontalGrabbed, 4, 1, 12,	 Point2f{ 0,0 }, scale,pTextureManager);
	m_pCharacterGrabbedDiagonal   =		new Sprite(CharacterDiagonalGrabbed, 6, 1, 20,      Point2f{ 0,0 }, scale, pTextureManager);
	m_pCharacterDead              =		new Sprite(CharacterDead, 8, 1, 12, Point2f{ 0,0 }, scale, pTextureManager);
	m_pCharacterDamaged			  =	    new Sprite(CharacterDamaged, 3, 1, 6, Point2f{ 0,0 }, scale, pTextureManager);

	m_pFlying = new Sprite(Flying, 2, 1, 0, Point2f{ 0,0 }, scale, pTextureManager);
	m_pRolling = new Sprite(Rolling, 4, 1, 10, Point2f{ 0,0 }, scale, pTextureManager);


	m_pInputManager = pInputManager;

	m_pPunchEffect = pSoundManager->GetSoundEffectPointer("Punch.wav");
	m_pGrabEffect = pSoundManager->GetSoundEffectPointer("Grab.wav");
	m_pJumpEffect = pSoundManager->GetSoundEffectPointer("Jump.wav");
	m_pDeathSound = pSoundManager->GetSoundEffectPointer("DeathSound.wav");

}

Character::~Character()
{
	
	delete    m_pCharacterIdle;
	m_pCharacterIdle = nullptr;

	delete	m_pCharacterRunning;
	m_pCharacterRunning = nullptr;

	delete	m_pCharacterJumping;
	m_pCharacterJumping = nullptr;

	delete	m_pCharacterFalling;
	m_pCharacterFalling = nullptr;

	delete	m_pSinglePunchAnimation;
	m_pSinglePunchAnimation = nullptr;

	delete	m_pCharacterDoublePunch;
	m_pCharacterDoublePunch = nullptr;

	delete	m_pCharacterTripleAttack;
	m_pCharacterTripleAttack = nullptr;

	delete	m_pCharacterGrabArm;
	m_pCharacterGrabArm = nullptr;


	delete	m_pCharacterGrabVertical;
	m_pCharacterGrabVertical = nullptr;

	delete	m_pCharacterGrabDiagonal;
	m_pCharacterGrabDiagonal = nullptr;

	delete	m_pCharacterGrabbedHorizontal;
	m_pCharacterGrabbedHorizontal = nullptr;

	delete	m_pCharacterGrabbedDiagonal;
	m_pCharacterGrabbedDiagonal = nullptr;

	delete	m_pCharacterDead;
	m_pCharacterDead = nullptr;

	delete m_pCharacterDamaged;
	m_pCharacterDamaged = nullptr;

	delete m_pCharacterGrabbedVertical;
	m_pCharacterGrabbedVertical = nullptr;


	delete m_pFlying;
	m_pFlying = nullptr;

	delete m_pRolling;
	m_pRolling = nullptr;


	delete m_pJumpAttack;
	m_pJumpAttack = nullptr;
}

void Character::Draw() const
{
	if (m_Invincible == true)
	{
		if (int(m_Timer * 500.0f) % 2 == 0)
		{
			DrawSprites();
		}
	}
	else
	{
		DrawSprites();
	}
}

void Character::Update(float elapsedsec, Level* level, const Rectf& characterPos)
{
	bool isAvatarOnGround = level->isOnGround(m_CharacterShape, m_CharacterVel);
	UpdateSprites(elapsedsec);
	HandleMovement(elapsedsec, level);


	const Uint8* pStates = SDL_GetKeyboardState(nullptr);



	HandleGatling(isAvatarOnGround,elapsedsec);
	HandleGrabbing(pStates);

	//if (m_pInputManager->IsKeyPressed(SDL_SCANCODE_D))
	//{
	//	if (pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_RIGHT])
	//	{
	//		m_CharacterActionState = CharacterActionState::diagionalGrabbing;
	//	}
	//	else if (pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_LEFT])
	//	{
	//		m_CharacterActionState = CharacterActionState::diagionalGrabbing;
	//	}
	//	else if (pStates[SDL_SCANCODE_UP])
	//	{
	//		m_CharacterActionState = CharacterActionState::verticalGrabbing;
	//	}
	//	else
	//	{
	//		m_CharacterActionState = CharacterActionState::horizontalGrabbing;
	//	}


	//	m_EnableInput = false;
	//}



	if (m_Flying == true && m_FlyTimer <= m_MaxFlyTime)
	{
		m_FlyTimer += elapsedsec;

	}
	if (m_FlyTimer > m_MaxFlyTime)
	{
		m_Flying = false;
		m_CharacterActionState = CharacterActionState::rolling;
		m_FlyTimer = 0.f;
		m_MaxFlyTime = m_OriginalFlyTime;
	}


	if (m_CharacterActionState != CharacterActionState::flying)
	{
		m_Damage = 1;
	}
	else
	{
		m_Damage = 5.f;
	}


	if (m_CurrentHealth <= 0)
	{
		m_EnableInput = false;

		m_CharacterActionState = CharacterActionState::dead;
	}


	if (m_pCharacterDamaged->IsOnLastFrame())
	{
		m_IsHit = false;
	}
	if (m_CharacterActionState != CharacterActionState::dead)
	{
		if (m_Timer > m_MaxInvincibiltyTimer)
		{
			m_Invincible = false;
			m_EnableInput = true;
			m_Timer = 0.f;
			m_MaxInvincibiltyTimer = m_OriginalMaximumInvincibleTimer;
		}

	}

	if (m_Invincible == true && m_Timer <= m_MaxInvincibiltyTimer)
	{
		m_Timer += elapsedsec;
		if (m_CharacterActionState != CharacterActionState::dead)
		{
			m_EnableInput = true;
		}
	}



}

const Rectf& Character::GetPlayerShape() const
{
	return m_CharacterShape;
}

const Rectf& Character::GetAttackShape() const
{
	return m_AttackRect;
}

void Character::SetPosition(const Point2f& pos)
{
	m_CharacterShape.left = pos.x;
	m_CharacterShape.bottom = pos.y;
}

void Character::SetEnableInput(bool EnableInput)
{
	m_EnableInput = EnableInput;

}


void Character::IsHit(float pointOfContact, float damage, float elapsedSec)
{
	if (m_Invincible == false)
	{
		if (!m_IsHit)
		{
			AddHealth(-damage);
		}

		if (pointOfContact < m_CharacterShape.left + (m_CharacterShape.width / 2.f))
		{
			m_CharacterVel.x = m_DashHorizontalSpeed;

		}
		if (pointOfContact > m_CharacterShape.left + (m_CharacterShape.width / 2.f))
		{

			m_CharacterVel.x = -m_DashHorizontalSpeed;
		}
		else
		{
			m_CharacterVel.x = m_DashHorizontalSpeed;

		}

		m_CharacterShape.left += m_CharacterVel.x * elapsedSec;

		SetInvincible();

		m_IsHit = true;
		m_CharacterActionState = CharacterActionState::damaged;

	}
}

void Character::AddHealth(float amount)
{
	m_CurrentHealth += amount;
	if (m_CurrentHealth > m_MaxHealth)
	{
		m_CurrentHealth = m_MaxHealth;
	}

}

float Character::GetDamage() const
{
	return m_Damage;
}

float Character::GetHealth() const
{
	return m_CurrentHealth;
}

float Character::GetMaxHealth() const
{
	return m_MaxHealth;
}

bool Character::IsHealthIncreased() const
{
	return m_IsHealthIncrease;
}

void Character::SetHealthIncreased(bool value)
{
	m_IsHealthIncrease = value;
}

bool Character::GetIsHit() const
{
	return m_IsHit;
}

const Vector2f& Character::GetCharacterVelocity() const
{
	return m_CharacterVel;
}

bool Character::GetIsLookingRight() const
{
	return m_LookingRight;
}

void Character::HandleMovement(float elapsedSec, Level* level)
{
	if (m_EnableInput)
	{
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);

		bool isAvatarOnGround = level->isOnGround(m_CharacterShape, m_CharacterVel);

		if (isAvatarOnGround) 
		{
			UpdateCharacterOnGround(pStates,elapsedSec);
		}
		else 
		{

			UpdateCharacterInAir(elapsedSec);

		}

		if (m_CharacterActionState == CharacterActionState::walking && isAvatarOnGround)
		{
			m_pCharacterJumping->SetCurrentFrame(0);
		}
		UpdateMovement(pStates, level);

		if (m_HasEnterdJumpingAttack)
		{
			m_CharacterActionState = CharacterActionState::jumpAttack;
		}


		m_CharacterShape.left += m_CharacterVel.x * elapsedSec;
		m_CharacterShape.bottom += m_CharacterVel.y * elapsedSec;

	}

	if (m_pCharacterGrabArm->IsOnLastFrame())
	{
		m_EnableInput = true;
	}
	if (m_pCharacterGrabVertical->IsOnLastFrame())
	{
		m_EnableInput = true;
	}
	if (m_pCharacterGrabDiagonal->IsOnLastFrame())
	{
		m_EnableInput = true;
	}

	level->HandleCollison(m_CharacterShape, m_CharacterVel);

}

void Character::HandleGrabbing(const Uint8* pStates)
{
	if (m_pInputManager->IsKeyPressed(SDL_SCANCODE_D))
	{
		if (pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_RIGHT])
		{
			m_CharacterActionState = CharacterActionState::diagionalGrabbing;
		}
		else if (pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_LEFT])
		{
			m_CharacterActionState = CharacterActionState::diagionalGrabbing;
		}
		else if (pStates[SDL_SCANCODE_UP])
		{
			m_CharacterActionState = CharacterActionState::verticalGrabbing;
		}
		else
		{
			m_CharacterActionState = CharacterActionState::horizontalGrabbing;
		}


		m_EnableInput = false;
	}

}

void Character::HandleGatling(bool isOnGround,float elapsedSec)
{
	if (isOnGround)
	{
		if (!(m_CharacterActionState == CharacterActionState::jumping) || !(m_CharacterActionState == CharacterActionState::falling))
		{
			m_NumberOfClicks = m_pInputManager->ClicksInRange(SDL_SCANCODE_S, 0.5f, elapsedSec);
		}


		if (m_NumberOfClicks == 1)
		{
			m_HasClickedOnce = true;
		}
		if (m_NumberOfClicks == 2)
		{
			m_HasClickedTwice = true;
		}
		if (m_NumberOfClicks == 3)
		{
			m_HasClickedThree = true;
		}
		if (m_NumberOfClicks > 3)
		{
			m_NumberOfClicks = 0;
		}


		if (m_HasClickedOnce)
		{
			m_EnableInput = false;
			m_CharacterActionState = CharacterActionState::singlePunch;
			if (m_pSinglePunchAnimation->IsOnLastFrame())
			{
				m_EnableInput = true;
				m_HasClickedOnce = false;

			}
		}
		else if (m_HasClickedTwice)
		{
			m_EnableInput = false;
			m_CharacterActionState = CharacterActionState::singlePunch;
			if (m_pSinglePunchAnimation->IsOnLastFrame())
			{
				m_CharacterActionState = CharacterActionState::doublePunch;
			}
		}
		else if (m_HasClickedThree)
		{
			m_EnableInput = false;
			m_CharacterActionState = CharacterActionState::singlePunch;

			if (m_pSinglePunchAnimation->IsOnLastFrame())
			{
				m_CharacterActionState = CharacterActionState::doublePunch;
			}
			if (m_pCharacterDoublePunch->IsOnLastFrame())
			{
				m_CharacterActionState = CharacterActionState::triplePunch;
			}

		}



	}
	else
	{
		if (m_pInputManager->IsKeyPressed(SDL_SCANCODE_S))
		{
			if (!m_HasEnterdJumpingAttack)
			{
				m_HasEnterdJumpingAttack = true;
			}
		}

		m_NumberOfClicks = 0;
		m_HasClickedOnce = false;
		m_HasClickedTwice = false;
		m_HasClickedThree = false;

	}

}

void Character::DrawSprites() const
{
	glPushMatrix(); 
	{
		glTranslatef(m_CharacterShape.left, m_CharacterShape.bottom, 0);
		if (m_LookingRight == true)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_CharacterShape.width, 0, 0);
		}

		switch (m_CharacterActionState)
		{
		case Character::CharacterActionState::alive:
			break;
		case Character::CharacterActionState::idle:

			m_pCharacterIdle->Draw();


			break;
		case Character::CharacterActionState::walking:
			m_pCharacterRunning->Draw();

			break;
		case Character::CharacterActionState::sprinting:
			break;
		case Character::CharacterActionState::jumping:
			m_pCharacterJumping->Draw();

			break;
		case Character::CharacterActionState::falling:
			m_pCharacterFalling->Draw();

			break;
		case Character::CharacterActionState::singlePunch:
			m_pSinglePunchAnimation->SetDrawPos(Point2f{ -70.f,-8.f });
			m_pSinglePunchAnimation->Draw();

			break;
		case Character::CharacterActionState::doublePunch:
			m_pCharacterDoublePunch->SetDrawPos(Point2f{-82.f,0  });
			m_pCharacterDoublePunch->Draw();

			break;
		case Character::CharacterActionState::triplePunch:
			m_pCharacterTripleAttack->SetDrawPos(Point2f{ -180.f,0 });
			m_pCharacterTripleAttack->Draw();
			break;
		case Character::CharacterActionState::jumpAttack:
			m_pJumpAttack->SetDrawPos(Point2f{ -60,-35.f });
			m_pJumpAttack->Draw();
			break;

		case Character::CharacterActionState::dead:
			m_pCharacterDead->Draw();
			break;
		case Character::CharacterActionState::horizontalGrabbing:
			m_pCharacterGrabArm->SetDrawPos(Point2f{ -365.f,0.0f });
			m_pCharacterGrabArm->Draw();

			break;
		case Character::CharacterActionState::horizontalGrabbed:
			m_pCharacterGrabbedHorizontal->SetDrawPos(Point2f{ -238.f, 0.0f });
			m_pCharacterGrabbedHorizontal->Draw();
			break;
		case Character::CharacterActionState::verticalGrabbing:
			m_pCharacterGrabVertical->SetDrawPos(Point2f{ -15,0.0f });
			m_pCharacterGrabVertical->Draw();
			break;
		case Character::CharacterActionState::verticalGrabbed:
			m_pCharacterGrabbedVertical->Draw();
			break;
		case Character::CharacterActionState::diagionalGrabbing:
			m_pCharacterGrabDiagonal->SetDrawPos(Point2f{ -260.f,0.0f });
			m_pCharacterGrabDiagonal->Draw();

			break;
		case Character::CharacterActionState::diagonalGrabbed:
			m_pCharacterGrabbedDiagonal->SetDrawPos(Point2f{ -200.f,0.0f });
			m_pCharacterGrabbedDiagonal->Draw();
			break;
		case Character::CharacterActionState::flying:
			m_pFlying->Draw();
			break;
		case Character::CharacterActionState::rolling:
			m_pRolling->Draw();
			break;

		case Character::CharacterActionState::damaged:
			m_pCharacterDamaged->Draw();
			break;

		}

	}
	glPopMatrix();
}

void Character::UpdateSprites(float elapsedSec)
{
	constexpr float Scale{ 4 };
	switch (m_CharacterActionState)
	{
	case Character::CharacterActionState::alive:
		break;
	case Character::CharacterActionState::idle:
		m_IsHit = false;
		UpdateHitboxes(m_pCharacterIdle->GetFrameWidth(), m_pCharacterIdle->GetFrameHeight(), Scale);
		m_pCharacterIdle->Update(elapsedSec);
		ResetAnimationFrames();

		m_pSinglePunchAnimation->SetCurrentFrame(0);
		m_pCharacterDoublePunch->SetCurrentFrame(0);
		m_pCharacterTripleAttack->SetCurrentFrame(0);
		m_pJumpAttack->SetCurrentFrame(0);
		m_pRolling->SetCurrentFrame(0);

		break;
	case Character::CharacterActionState::walking:
		m_IsHit = false;

		m_pCharacterRunning->Update(elapsedSec);

		ResetAnimationFrames();
		m_pJumpAttack->SetCurrentFrame(0);

		m_pSinglePunchAnimation->SetCurrentFrame(0);
		m_pCharacterDoublePunch->SetCurrentFrame(0);
		m_pCharacterTripleAttack->SetCurrentFrame(0);


		if (m_pCharacterFalling->IsOnLastFrame())
		{
			m_pCharacterFalling->SetCurrentFrame(0);
		}

		break;
	case Character::CharacterActionState::sprinting:
		break;
	case Character::CharacterActionState::jumping:
		m_pCharacterDamaged->SetCurrentFrame(0);
		m_pJumpAttack->SetCurrentFrame(0);
		m_pCharacterGrabArm->SetCurrentFrame(0);
		m_pSinglePunchAnimation->SetCurrentFrame(0);
		m_pCharacterGrabVertical->SetCurrentFrame(0);
		m_pCharacterGrabDiagonal->SetCurrentFrame(0);
		m_pCharacterGrabbedHorizontal->SetCurrentFrame(0);
		m_pCharacterDoublePunch->SetCurrentFrame(0);
		m_pCharacterTripleAttack->SetCurrentFrame(0);

		

		if (!m_pCharacterJumping->IsOnLastFrame())
		{
			m_pCharacterJumping->Update(elapsedSec);
		}
		else
		{
			m_pCharacterJumping->HoldLastFrame();
		}

		break;
	case Character::CharacterActionState::falling:
		m_pCharacterDamaged->SetCurrentFrame(0);
		m_pJumpAttack->SetCurrentFrame(0);

		m_pJumpAttack->SetCurrentFrame(0);
		m_pCharacterGrabArm->SetCurrentFrame(0);
		m_pCharacterGrabVertical->SetCurrentFrame(0);
		m_pSinglePunchAnimation->SetCurrentFrame(0);
		m_pCharacterJumping->SetCurrentFrame(0);
		m_pCharacterGrabDiagonal->SetCurrentFrame(0);
		m_pCharacterGrabbedHorizontal->SetCurrentFrame(0);
		m_pCharacterDoublePunch->SetCurrentFrame(0);
		m_pCharacterTripleAttack->SetCurrentFrame(0);



		if (!m_pCharacterFalling->IsOnLastFrame())
		{
			m_pCharacterFalling->Update(elapsedSec);
		}
		else
		{
			m_pCharacterFalling->HoldLastFrame();
		}


		break;
	case Character::CharacterActionState::singlePunch:
		m_pCharacterDamaged->SetCurrentFrame(0);
		m_pCharacterDoublePunch->SetCurrentFrame(0);
		m_pCharacterTripleAttack->SetCurrentFrame(0);
		m_pJumpAttack->SetCurrentFrame(0);

		if (!m_pSinglePunchAnimation->IsOnLastFrame())
		{
			m_pSinglePunchAnimation->Update(elapsedSec);

		}

		if ((m_pSinglePunchAnimation->GetCurrentFrame() == 2))
		{
			m_AttackRect.bottom = (m_CharacterShape.bottom + m_CharacterShape.height * 0.45f);
			if (m_LookingRight)
			{
				m_AttackRect.left = m_CharacterShape.left + m_CharacterShape.width;
			}
			else
			{
				m_AttackRect.left = m_CharacterShape.left - m_AttackRect.width;
			}

		}
		else
		{
			m_AttackRect.left = -500.0f;
			m_AttackRect.bottom = -500.0f;
			m_AttackRect.width = 75;
			m_AttackRect.height = 40;

		}


		break;

	case Character::CharacterActionState::doublePunch:
		m_pJumpAttack->SetCurrentFrame(0);

		if (m_HasClickedOnce)
		{
			m_pSinglePunchAnimation->SetCurrentFrame(0);

		}
		if (!m_pCharacterDoublePunch->IsOnLastFrame())
		{
			m_pCharacterDoublePunch->Update(elapsedSec);
		}
		else
		{
			m_EnableInput = true;
			m_HasClickedTwice = false;
		}


		if ((m_pCharacterDoublePunch->GetCurrentFrame() == 2))
		{
			m_AttackRect.bottom = (m_CharacterShape.bottom + m_CharacterShape.height * 0.5f);
			if (m_LookingRight)
			{
				m_AttackRect.left = m_CharacterShape.left + m_CharacterShape.width;
			}
			else
			{
				m_AttackRect.left = m_CharacterShape.left - m_AttackRect.width;
			}

		}
		else
		{
			m_AttackRect.left = -500.0f;
			m_AttackRect.bottom = -500.0f;
			m_AttackRect.width = 75;
			m_AttackRect.height = 40;

		}

		break;
	case Character::CharacterActionState::triplePunch:
		if (m_HasClickedTwice)
		{
			m_pCharacterDoublePunch->SetCurrentFrame(0);
		}
		if (!m_pCharacterTripleAttack->IsOnLastFrame())
		{
			m_pCharacterTripleAttack->Update(elapsedSec);
		}
		else
		{
			m_EnableInput = true;
			m_HasClickedThree = false;
		}



		if ((m_pCharacterTripleAttack->GetCurrentFrame() == 2) || (m_pCharacterTripleAttack->GetCurrentFrame() == 4) || (m_pCharacterTripleAttack->GetCurrentFrame() == 6) || (m_pCharacterTripleAttack->GetCurrentFrame() == 9))
		{

			m_AttackRect.bottom = (m_CharacterShape.bottom + m_CharacterShape.height * 0.2f);
			m_AttackRect.width = 150.0f;
			m_AttackRect.height = 150.0f;
			if (m_LookingRight)
			{
				m_AttackRect.left = m_CharacterShape.left + m_CharacterShape.width;
			}
			else
			{
				m_AttackRect.left = m_CharacterShape.left - m_AttackRect.width;
			}

		}
		else
		{
			m_pPunchEffect->SetVolume(0);

			m_AttackRect.left = -500.0f;
			m_AttackRect.bottom = -500.0f;
			m_AttackRect.width = 75;
			m_AttackRect.height = 40;

		}


		break;

	case Character::CharacterActionState::jumpAttack:
		if (!m_pJumpAttack->IsOnLastFrame())
		{
			m_pJumpAttack->Update(elapsedSec);

		}
		else
		{
			m_HasEnterdJumpingAttack = false;
			m_CharacterActionState = CharacterActionState::falling;
		}

		if ((m_pJumpAttack->GetCurrentFrame() == 4))
		{

			m_AttackRect.width = 100.0f;
			m_AttackRect.height = 150.0f;
			m_AttackRect.bottom = m_CharacterShape.bottom - (m_AttackRect.height / 2.f);
			m_AttackRect.left = m_CharacterShape.left + (m_AttackRect.width / 2.f);
			if (m_LookingRight)
			{
				m_AttackRect.left = m_CharacterShape.left + m_CharacterShape.width;
			}
			else
			{
				m_AttackRect.left = m_CharacterShape.left - m_AttackRect.width;
			}

		}
		else
		{
			m_AttackRect.left = -500.0f;
			m_AttackRect.bottom = -500.0f;
			m_AttackRect.width = 75;
			m_AttackRect.height = 40;

		}

		break;

	case Character::CharacterActionState::dead:

		m_pCharacterDamaged->SetCurrentFrame(0);
		m_pCharacterDoublePunch->SetCurrentFrame(0);
		m_pCharacterTripleAttack->SetCurrentFrame(0);


		if (!m_pCharacterDead->IsOnLastFrame())
		{
			m_pCharacterDead->Update(elapsedSec);
		}

		break;
	case Character::CharacterActionState::horizontalGrabbing:
		m_pJumpAttack->SetCurrentFrame(0);
		m_pCharacterDoublePunch->SetCurrentFrame(0);
		m_pCharacterTripleAttack->SetCurrentFrame(0);

		if (m_pCharacterGrabArm->GetCurrentFrame() == 3)
		{
			m_pGrabEffect->Play(0);

		}
		m_pCharacterGrabArm->Update(elapsedSec);
		m_pCharacterGrabbedHorizontal->SetCurrentFrame(0);
		m_pCharacterDamaged->SetCurrentFrame(0);

		switch (m_pCharacterGrabArm->GetCurrentFrame())
		{
		case 8:
			m_MaxReachPoint.x = -m_CharacterShape.width + 50.f;
			m_MaxReachPoint.y = m_CharacterShape.height * 0.32f;

			break;
		case 9:
			m_MaxReachPoint.x = -m_CharacterShape.width - 120.f;
			m_MaxReachPoint.y = m_CharacterShape.height * 0.325f;
			break;
		case 10:
			m_MaxReachPoint.x = -m_CharacterShape.width + 8.f;
			m_MaxReachPoint.y = m_CharacterShape.height * 0.325f;
			break;

		}


		break;
	case Character::CharacterActionState::horizontalGrabbed:
		m_pJumpAttack->SetCurrentFrame(0);
		m_pCharacterDamaged->SetCurrentFrame(0);
		m_pCharacterDoublePunch->SetCurrentFrame(0);
		m_pCharacterTripleAttack->SetCurrentFrame(0);

		if (!m_pCharacterGrabbedHorizontal->IsOnLastFrame())
		{
			m_pCharacterGrabbedHorizontal->Update(elapsedSec);
			if (m_pCharacterGrabbedHorizontal->GetCurrentFrame() > 1)
			{
				if (m_LookingRight == false)
				{
					m_CharacterShape.left -= m_DashHorizontalSpeed * elapsedSec;

				}
				else
				{
					m_CharacterShape.left += m_DashHorizontalSpeed * elapsedSec;
				}
			}
		}
		else
		{
			//m_EnableInput = true;
			m_CharacterActionState = CharacterActionState::flying;
		}

		break;
	case Character::CharacterActionState::verticalGrabbing:
		m_pCharacterGrabVertical->Update(elapsedSec);
		m_pCharacterDamaged->SetCurrentFrame(0);
		m_pJumpAttack->SetCurrentFrame(0);

		m_pCharacterTripleAttack->SetCurrentFrame(0);

		if (m_pCharacterGrabVertical->GetCurrentFrame() == 2)
		{
			m_pGrabEffect->Play(0);

		}

		switch (m_pCharacterGrabVertical->GetCurrentFrame())
		{
		case 3:
			m_MaxReachPoint.x = 28.f;
			m_MaxReachPoint.y = m_CharacterShape.height;
			break;
		case 4:
			m_MaxReachPoint.x = 28.f;
			m_MaxReachPoint.y = m_CharacterShape.height+180.f;
			break;
		case 5:
			m_MaxReachPoint.x = 28.f;
			m_MaxReachPoint.y = m_CharacterShape.height + 50.f;

			break;

		}

		break;
	case Character::CharacterActionState::verticalGrabbed:
		m_pCharacterDamaged->SetCurrentFrame(0);
		m_pCharacterTripleAttack->SetCurrentFrame(0);
		m_pCharacterGrabbedVertical->SetDrawPos(Point2f{ 30.0f,0 });
		if (!m_pCharacterGrabbedVertical->IsOnLastFrame())
		{
			m_pCharacterGrabbedVertical->Update(elapsedSec);
			if (m_pCharacterGrabbedVertical->GetCurrentFrame() > 1)
			{
					m_CharacterShape.bottom += m_DashHorizontalSpeed * elapsedSec;
			}
		}
		else
		{
			m_CharacterActionState = CharacterActionState::flying;
		}

		break;
	case Character::CharacterActionState::diagionalGrabbing:
		m_pCharacterDamaged->SetCurrentFrame(0);
		if (m_pCharacterGrabDiagonal->GetCurrentFrame() == 2)
		{
			m_pGrabEffect->Play(0);
		}

		m_pCharacterGrabDiagonal->Update(elapsedSec);
		m_pCharacterGrabbedDiagonal->SetCurrentFrame(0);

		break;
	case Character::CharacterActionState::diagonalGrabbed:
		m_pCharacterDamaged->SetCurrentFrame(0);

		if (!m_pCharacterGrabbedDiagonal->IsOnLastFrame())
		{
			m_pCharacterGrabbedDiagonal->Update(elapsedSec);

			if (m_pCharacterGrabbedDiagonal->GetCurrentFrame() > 0)
			{
				if (m_LookingRight == false)
				{
						m_CharacterShape.left -= m_DashHorizontalSpeed * elapsedSec;
						m_CharacterShape.bottom += m_DashVerticalSpeed * elapsedSec;

				}
				else
				{
						m_CharacterShape.left += m_DashHorizontalSpeed * elapsedSec;
						m_CharacterShape.bottom += m_DashVerticalSpeed * elapsedSec;

				}
			}
		}
		else
		{
			m_CharacterActionState = CharacterActionState::flying;
		}
		break;
	case Character::CharacterActionState::flying:
		

		m_pRolling->SetCurrentFrame(0);
		m_Flying = true;
		
		if (m_pCharacterGrabbedVertical->IsOnLastFrame())
		{
			m_pFlying->SetCurrentFrame(1);
			m_pFlying->SetDrawPos(Point2f{ -100,0 });
			m_CharacterShape.bottom += ((m_DashHorizontalSpeed)/3.f) * elapsedSec;

		}
		else
		{
			m_pFlying->SetCurrentFrame(0);
			if (m_LookingRight == false)
			{
				m_AttackRect.width = 50.0f;
				m_AttackRect.height = m_CharacterShape.height;
				m_AttackRect.bottom = m_CharacterShape.bottom;
				m_AttackRect.left = m_CharacterShape.left - m_AttackRect.width;

				m_CharacterShape.left -= m_DashHorizontalSpeed * elapsedSec;

			}
			else
			{


				m_CharacterShape.left += m_DashHorizontalSpeed * elapsedSec;
				m_AttackRect.width = 50.0f;
				m_AttackRect.height = m_CharacterShape.height;
				m_AttackRect.bottom = m_CharacterShape.bottom;
				m_AttackRect.left = m_CharacterShape.left + m_CharacterShape.width;

			}

		}
		break;
	case Character::CharacterActionState::rolling:
		m_AttackRect.left = 0;
		m_AttackRect.bottom = 0;
		if (m_pFlying->GetCurrentFrame() == 1)
		{
			m_EnableInput = true;
			m_CharacterActionState = CharacterActionState::idle;

		}
		else if (!m_pRolling->IsOnLastFrame())
		{
			m_pRolling->Update(elapsedSec);

			if (m_LookingRight == false)
			{
				m_CharacterShape.left -= ((m_DashHorizontalSpeed)/4.f) * elapsedSec;

			}
			else
			{
				m_CharacterShape.left += ((m_DashHorizontalSpeed) / 4.f) * elapsedSec;
			}

		}
		else
		{
			m_EnableInput = true;
			m_CharacterActionState = CharacterActionState::idle;
		}

		break;

	case Character::CharacterActionState::startTransition:
		break;
	case Character::CharacterActionState::damaged:
		if (!m_pCharacterDamaged->IsOnLastFrame())
		{
			m_pCharacterDamaged->Update(elapsedSec);

		}
		else
		{
			m_CharacterActionState = CharacterActionState::idle;
		}
		break;

	}
}

void Character::UpdateHitboxes(float width, float height, float scale)
{
	m_CharacterShape.width = width * scale,
	m_CharacterShape.height = height * scale;
}

void Character::HandleGrabbing(float elapsedSec, GameObjectMananger* gameObjectManager)
{
	Point2f beginGrabPoint{ 0,0 };
	Point2f endGrabPoint{ 0,0 };

	utils::HitInfo hitInfo{};


	if (m_CharacterActionState == CharacterActionState::horizontalGrabbing)
	{
		beginGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 0.6f;
		beginGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 0.6f;

		if (m_LookingRight == true)
		{
			endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 0.6f;
			switch (m_pCharacterGrabArm->GetCurrentFrame())
			{
			case 6:
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 1.5f;

				break;
			case 7:
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 1.7f;

				break;
			case 8:
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 1.9f;
				break;
			case 9:
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 2.3f;
				break;
			case 10:
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 2.5f;
				break;
			case 11:
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 2.7f;

				break;
			case 12:
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 2.9f;
				break;
			}

		}
		else
		{
			endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 0.6f;
			switch (m_pCharacterGrabArm->GetCurrentFrame())
			{
			case 6:
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 0.5f;

				break;
			case 7:
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 0.7f;

				break;
			case 8:
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 0.9f;
				break;
			case 9:
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 1.3f;
				break;
			case 10:
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 1.6f;
				break;
			case 11:
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 1.8f;

				break;
			case 12:
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 2;
				break;
			}

		}

	}
	if (m_CharacterActionState == CharacterActionState::verticalGrabbing)
	{
		beginGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 0.45f;
		beginGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 0.8f;
		endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 0.45f;

			switch (m_pCharacterGrabVertical->GetCurrentFrame())
			{
			case 5:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 0.8f;

				break;
			case 6:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 1.5f;

				break;
			case 7:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 1.7f;
				break;
			case 8:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 2.1f;
				break;
			case 9:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 2.3f;
				break;
			case 10:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 2.6f;

				break;
			case 11:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 2.5f;
				break;
			}

		
	}
	if (m_CharacterActionState == CharacterActionState::diagionalGrabbing)
	{

		if (m_LookingRight == true)
		{
			beginGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 0.6f;
			beginGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 0.8f;

			switch (m_pCharacterGrabDiagonal->GetCurrentFrame())
			{
			case 6:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 1.1f;
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 1.1f;

				break;
			case 7:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 1.3f;
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 1.3f;

				break;
			case 8:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 1.7f;
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 1.75f;

				break;
			case 9:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 2.1f;
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 2.3f;

				break;
			case 10:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 2.3f;
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 2.5f;

				break;
			case 11:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 0.8f;
				endGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 0.6f;

				break;
			}

		}
		else
		{
			beginGrabPoint.x = m_CharacterShape.left + m_CharacterShape.width * 0.4f;
			beginGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 0.8f;

			switch (m_pCharacterGrabDiagonal->GetCurrentFrame())
			{
			case 6:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 1.1f;
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 0.f;

				break;
			case 7:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 1.3f;
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 0.25f;

				break;
			case 8:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 1.7f;
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 0.7f;

				break;
			case 9:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 2.1f;
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 1.2f;

				break;
			case 10:
				endGrabPoint.y = m_CharacterShape.bottom + m_CharacterShape.height * 2.3f;
				endGrabPoint.x = m_CharacterShape.left - m_CharacterShape.width * 1.4f;

				break;
			}


		}

	}


	for (GameObject* gameObject : gameObjectManager->GetObjectVector())
	{



		if (gameObjectManager->GetCanBeGrabbed())
		{

			if (utils::Raycast(gameObject->GetPoleVertices(),beginGrabPoint,endGrabPoint,hitInfo))
			{
				float Distance = utils::GetDistance(hitInfo.intersectPoint, Point2f{ m_CharacterShape.left,m_CharacterShape.bottom });
				if (hitInfo.lambda > 0.5f)
				{
					if (m_CharacterActionState == CharacterActionState::horizontalGrabbing)
					{
						if (Distance < 200)
						{
							m_pCharacterGrabbedHorizontal->SetCurrentFrame(3);

						}
						else if (Distance < 350 )
						{
							m_pCharacterGrabbedHorizontal->SetCurrentFrame(2);
						}
						else
						{
							m_pCharacterGrabbedVertical->SetCurrentFrame(0);

						}

						m_CharacterActionState = CharacterActionState::horizontalGrabbed;

					}
					if (m_CharacterActionState == CharacterActionState::verticalGrabbing)
					{

						if (Distance < 200)
						{
							m_pCharacterGrabbedVertical->SetCurrentFrame(4);
						}
						else
						{
							m_pCharacterGrabbedVertical->SetCurrentFrame(0);
						}
						m_CharacterActionState = CharacterActionState::verticalGrabbed;

					}
					if (m_CharacterActionState == CharacterActionState::diagionalGrabbing)
					{
						if (Distance < 400)
						{
							m_pCharacterGrabbedDiagonal->SetCurrentFrame(2);
						}
						else
						{
							m_pCharacterGrabbedDiagonal->SetCurrentFrame(0);
						}
						m_CharacterActionState = CharacterActionState::diagonalGrabbed;

					}

				}

			}
		}


	}

}

void Character::SetActionSate(const CharacterActionState& state)
{
	m_CharacterActionState = state;
}

Character::CharacterActionState Character::GetSate()const
{
	return m_CharacterActionState;
}

void Character::ResetActionState()
{
	m_CharacterActionState != m_CharacterActionState;
	
}

void Character::ResetAnimationFrames() 
{
	m_pCharacterDamaged->SetCurrentFrame(0);
	m_pCharacterJumping->SetCurrentFrame(0);
	m_pCharacterFalling->SetCurrentFrame(0);
	m_pSinglePunchAnimation->SetCurrentFrame(0);
	m_pCharacterGrabVertical->SetCurrentFrame(0);
	m_pCharacterGrabArm->SetCurrentFrame(0);
	m_pCharacterGrabDiagonal->SetCurrentFrame(0);
	m_pCharacterGrabbedHorizontal->SetCurrentFrame(0); 
	m_pCharacterGrabbedDiagonal->SetCurrentFrame(0);
	m_pCharacterGrabbedVertical->SetCurrentFrame(0);
	m_pCharacterGrabbedVertical->SetCurrentFrame(0);
}

void Character::UpdateCharacterOnGround(const Uint8* keyboardState, float elapsedSec) 
{
	m_CharacterVel = Vector2f{ 0,0 };
	m_CharacterActionState = CharacterActionState::idle;

	if (keyboardState[SDL_SCANCODE_SPACE])
	{
		m_CharacterVel.y = m_JumpSpeed;
		m_CharacterActionState = CharacterActionState::jumping;
	}

}

void Character::UpdateCharacterInAir(float elapsedSeconds) 
{
	m_CharacterVel += m_CharacterAccel * elapsedSeconds;

	if ((m_CharacterActionState == CharacterActionState::jumping || m_CharacterActionState == CharacterActionState::falling) && m_CharacterVel.y < 0.0f)
	{
		m_CharacterActionState = CharacterActionState::falling;
	}
	else 
	{
		m_CharacterActionState = CharacterActionState::jumping;
	}
}

void Character::UpdateMovement(const Uint8* keyboardState,Level* level)
{
	float direction = 0.0f;
	bool isAvatarOnGround = level->isOnGround(m_CharacterShape, m_CharacterVel);

	if (keyboardState[SDL_SCANCODE_RIGHT])
	{
		direction = 1.0f;
		m_LookingRight = true;
	}
	else if (keyboardState[SDL_SCANCODE_LEFT])
	{
		direction = -1.0f;
		m_LookingRight = false;
	}

	m_CharacterVel.x = direction * m_HorizontalSpeed;
	UpdateCharacterActionState(isAvatarOnGround);
}

void Character::UpdateCharacterActionState(bool isOnGround)
{
	if (isOnGround)
	{
		if (m_CharacterVel.x != 0.0f)
		{
			m_CharacterActionState = CharacterActionState::walking;
		}
		else
		{
			m_CharacterActionState = CharacterActionState::idle;
		}
	}
	else
	{
		if (m_CharacterVel.y < 0.0f)
		{
				m_CharacterActionState = CharacterActionState::falling;
		}
		else
		{
			m_CharacterActionState = CharacterActionState::jumping;
		}
	}
}

void Character::HandleAttacks(float elapsedSec)
{

}

void Character::SetInvincible()
{
	m_Invincible = true;
	m_Timer = 0.f;
}
