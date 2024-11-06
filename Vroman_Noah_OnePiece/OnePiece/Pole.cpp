#include "pch.h"
#include "Pole.h"
#include <utils.h>

Pole::Pole(const Rectf& pos)
	:GameObject(pos)
{
	m_PoleRect.left = 0;
	m_PoleRect.bottom = 0;
	m_PoleRect.width = 0;
	m_PoleRect.height = 0;

	m_Vertices.push_back(Point2f{ pos.left,pos.bottom });
	m_Vertices.push_back(Point2f{ pos.left + pos.width,pos.bottom });
	m_Vertices.push_back(Point2f{ pos.left + pos.width,pos.bottom + pos.height });
	m_Vertices.push_back(Point2f{ pos.left,pos.bottom + pos.height });
	m_Vertices.push_back(Point2f{ pos.left,pos.bottom });
}

Pole::~Pole()
{

}


const Rectf& Pole::GetObjectShape() const
{
	return m_PoleRect;
}

const std::vector<Point2f>& Pole::GetPoleVertices()
{
	return m_Vertices;
}

