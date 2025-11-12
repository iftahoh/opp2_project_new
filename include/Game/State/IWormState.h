#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

/*
IWormState interface representing the state of a Worm object.
*/

class Worm;

class IWormState {
public:
    virtual ~IWormState() = default;
    virtual void onEnter(Worm& worm) = 0;
    virtual void handleInput(Worm& worm, const sf::Event& event) = 0;
    virtual void update(Worm& worm, sf::Time deltaTime) = 0;
    virtual void render(sf::RenderWindow& window, Worm& worm) {};
    virtual void onExit(Worm& worm) = 0;
};