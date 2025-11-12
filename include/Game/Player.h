#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Game/Object/Worm.h"

class GameController;

class Player {
public:
 
    Player(b2World& world, GameController& gameController, const sf::Vector2f& basePosition, sf::Color color);
    Player(b2World& world, GameController& gameController, const sf::Vector2f& basePosition, sf::Color color, int wormNum);
    void handleInput(const sf::Event& event);
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);
	sf::Vector2f getPosition() const;

    int getTotalHealth() const;

    const std::vector<std::string>& getWeaponInventory() const;
    int getSelectedWeaponIndex() const;

private:
    void nextWorm();

    std::vector<Worm*> m_worms;
    int m_currentWormIndex;

    GameController& m_gameController;
	sf::Color m_color;

    std::vector<std::string> m_weaponInventory; // רשימת הנשקים (לפי שם)
    int m_selectedWeaponIndex; // האינדקס לנשק שנבחר כרגע
};