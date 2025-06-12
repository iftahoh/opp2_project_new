#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

#include "Macros.h"

class ResourceGraphic

{
public:
	static ResourceGraphic& getInstance();
	sf::Texture& getTexture(const std::string& name);
	sf::Font& getFont(const std::string& name);

	ResourceGraphic(const ResourceGraphic&) = delete;
	ResourceGraphic& operator=(const ResourceGraphic&) = delete;


private:
	ResourceGraphic();

	void loadTextures(const std::string& name, const std::string& filename);
	void loadFonts(const std::string& name, const std::string& filename);

	std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
	std::unordered_map<std::string, std::unique_ptr<sf::Font>> m_fonts;
};
