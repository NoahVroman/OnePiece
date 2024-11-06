#include "pch.h"
#include "GameObject.h"
#include <utils.h>

GameObject::GameObject(const Rectf& pos)
	:m_GameObjectPos{pos}
	,m_isHit{ false }

{
}

GameObject::~GameObject()
{
}

void GameObject::IsHit(const Rectf& characterAttackShape)
{
	if (!m_isHit)
	{
		if (utils::IsOverlapping(GetObjectShape(), characterAttackShape))
		{
			m_isHit = true;
		}
	}
 
}
