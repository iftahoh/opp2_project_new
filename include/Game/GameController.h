#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <memory>

class GameObject;
class Player;

class GameController {
public:
    explicit GameController(sf::RenderWindow& window);
    ~GameController();
    void run();

    void addGameObject(std::unique_ptr<GameObject> object);
    b2World& getWorld();

private:
    void handleEvents();
    void update(sf::Time deltaTime);
    void render();
    void setupWorld();

    sf::RenderWindow& m_window;
    b2World m_world;

    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::vector<std::unique_ptr<Player>> m_players;
    int m_currentPlayerIndex;

    sf::Texture m_skyTexture;
    sf::Sprite m_skySprite;
};