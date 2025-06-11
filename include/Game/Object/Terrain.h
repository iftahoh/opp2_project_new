// include/GameObjects/Terrain.h
#pragma once
#include "Game/Object/StaticObject.h"
#include <vector>

class Terrain : public StaticObject {
public:
    Terrain(b2World& world, const sf::Vector2u& windowSize);
    void render(sf::RenderWindow& window) override;

private:
    sf::VertexArray m_terrainDrawable;
    sf::Texture m_terrainTexture;
    static constexpr float SCALE = 30.0f;
};