#pragma once
#include<unordered_map>
class Texture;

class TextureManager final
{
public:
	TextureManager()=default;
	~TextureManager();
	TextureManager(const TextureManager& other) = delete;
	TextureManager& operator=(const TextureManager& other) = delete;
	TextureManager(TextureManager&& other) = delete;
	TextureManager& operator=(TextureManager&& other) = delete;

	Texture* GetSpritePointer(const std::string& filename);

private:

	std::unordered_map<std::string, Texture*> m_Map;



};

