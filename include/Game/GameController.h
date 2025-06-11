#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>

class GameController {
public:
    explicit GameController(sf::RenderWindow& window);
    void run();

private:
    void handleEvents();
    void update(sf::Time deltaTime);
    void render();
    void setupWorld();
    void setupGraphics();

    static constexpr float SCALE = 30.0f;

    sf::RenderWindow& m_window;
    b2World m_world;

    // Terrain members
    std::vector<b2Vec2> m_terrainPhysicsVertices;
    sf::Texture m_terrainTexture;
    sf::VertexArray m_terrainDrawable;

    // Ball members
    b2Body* m_ballBody;
    sf::CircleShape m_ballShape;

    // === הוספנו משתנים עבור רקע השמיים ===
    sf::Texture m_skyTexture;
    sf::Sprite m_skySprite;
};