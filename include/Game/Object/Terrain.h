// include/GameObjects/Terrain.h
#pragma once
#include "Game/Object/StaticObject.h"
#include <vector>
#include <box2d/b2_math.h>

class Terrain : public StaticObject {
public:
    Terrain(b2World& world, const sf::Vector2u& windowSize);
    void render(sf::RenderWindow& window) override;
    float getSurfaceY(float x) const;

private:
    sf::VertexArray m_terrainDrawable;
    sf::Texture m_terrainTexture;
    std::vector<b2Vec2> m_surfacePoints;
    static constexpr float SCALE = 30.0f;
};