// include/Game/GameController.h
#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <memory>

// Forward declarations
class GameObject;
class Player; // הוספת Forward Declaration

class GameController {
public:
    explicit GameController(sf::RenderWindow& window);
    ~GameController();
    void run();

    // פונקציה ציבורית שתאפשר ל-Player להוסיף אובייקטים
    void addGameObject(std::unique_ptr<GameObject> object);

private:
    void handleEvents(const sf::Event& event);
    void update(sf::Time deltaTime);
    void render();
    void setupWorld();

    sf::RenderWindow& m_window;
    b2World m_world;

    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::vector<std::unique_ptr<Player>> m_players; // וקטור של שחקנים
    int m_currentPlayerIndex; // מי השחקן הנוכחי

    // ... שאר המשתנים נשארים
    sf::Texture m_skyTexture;
    sf::Sprite m_skySprite;
};