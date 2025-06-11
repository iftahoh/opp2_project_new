#include "ResourceGraphic.h"

ResourceGraphic& ResourceGraphic::getInstance()
{
	static ResourceGraphic instance;
	return instance;
}

sf::Texture& ResourceGraphic::getTexture(const std::string& name)
{
    auto found = m_textures.find(name);
    if (found == m_textures.end()) {
        throw std::runtime_error("Texture not found: " + name);
    }
    return *found->second;
}

sf::Font& ResourceGraphic::getFont(const std::string& name)
{
	auto found = m_fonts.find(name);
	if (found == m_fonts.end()) {
		throw std::runtime_error("Font not found: " + name);
	}
	return *found->second;

}

void ResourceGraphic::loadTextures(const std::string& name, const std::string& filename)
{
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filename + ".png")) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }
    m_textures[name] = std::move(texture);
}

void ResourceGraphic::loadFonts(const std::string& name, const std::string& filename)
{
	auto font = std::make_unique<sf::Font>();
	if (!font->loadFromFile(filename + ".ttf")) {
		throw std::runtime_error("Failed to load font: " + filename);
	}
	m_fonts[name] = std::move(font);
}

ResourceGraphic::ResourceGraphic()
{
    for (int index = 0; index < TEXTURE_PATH_SIZE; index++)
    {
        loadTextures(RESOURCES_TEXTURE_PATH[index], RESOURCES_TEXTURE_PATH[index]);
    }
    loadFonts("main_font", "arial");
}
