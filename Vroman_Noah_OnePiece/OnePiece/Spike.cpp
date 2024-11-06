#include "pch.h"
#include "Spike.h"
#include <utils.h>
Spike::Spike(const Rectf& shape)
	:m_Shape{shape}
	,m_Damage{ 1 }
{
}

Spike::~Spike()
{

}


bool Spike::IsOverlapping(const Rectf& otherShape)
{
	if (utils::IsOverlapping(m_Shape, otherShape))
	{
		return true;
	}
	return false;
}

const Rectf& Spike::GetShape()const
{
	return m_Shape;
}

const int Spike::GetDamage() const
{
	return m_Damage;
}

void Spike::SetShape(const Rectf& other)
{
	m_Shape = other;
}
