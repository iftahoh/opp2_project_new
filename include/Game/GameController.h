// include/Game/GameController.h
#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <memory>

// Forward declaration
class GameObject;

class GameController {
public:
    explicit GameController(sf::RenderWindow& window);
    ~GameController();
    void run();

private:
    void handleEvents();
    void update(sf::Time deltaTime);
    void render();
    void setupWorld();

    sf::RenderWindow& m_window;
    b2World m_world;

    // וקטור פולימורפי שמחזיק את כל אובייקטי המשחק
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;

    // רקע
    sf::Texture m_skyTexture;
    sf::Sprite m_skySprite;
};