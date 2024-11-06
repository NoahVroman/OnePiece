#include "pch.h"
#include <Texture.h>
#include "TextureManager.h"
#include <iostream>

TextureManager::~TextureManager()
{
	for (const std::pair<std::string, Texture*>& pair : m_Map)
	{
		
		delete pair.second;
	}
	m_Map.clear();
}

Texture* TextureManager::GetSpritePointer(const std::string& filename)
{
	Texture* texture;
	std::unordered_map<std::string, Texture*>::iterator result;

	result = m_Map.find(filename);

	if (result == m_Map.end())
	{
		texture = new Texture(filename);
		m_Map[filename] = texture;
	}
	else
	{
		texture = result->second;
	}
	return texture;
}
