#pragma once
#include <SFML/Graphics.hpp>

class Worm;
class GameController;

class IWeapon {
public:
    virtual ~IWeapon() = default;

    virtual void handleInput(const sf::Event& event, Worm& worm) = 0;
    virtual void update(Worm& worm, GameController& controller) = 0;
    virtual void render(sf::RenderWindow& window, const Worm& worm) = 0;
};