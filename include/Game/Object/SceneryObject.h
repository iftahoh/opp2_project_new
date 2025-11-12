#pragma once

#include "Game/Object/StaticObject.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>

class SceneryObject : public StaticObject {
public:
    // הבנאי מקבל עולם, מיקום, ואת שם הטקסטורה לטעינה
    SceneryObject(b2World& world, const sf::Vector2f& position, const std::string& textureKey);

    void render(sf::RenderWindow& window) override;

private:
    sf::Texture m_texture; // האובייקט יחזיק טקסטורה משלו
    sf::Sprite m_sprite;   // הספריט שישתמש בטקסטורה
    static constexpr float SCALE = 30.0f;
};