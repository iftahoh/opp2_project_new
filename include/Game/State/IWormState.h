#pragma once

#include <SFML/System/Time.hpp>

class Worm;

class IWormState {
public:
    virtual ~IWormState() = default;
    virtual void onEnter(Worm& worm) = 0;
    virtual void handleInput(Worm& worm) = 0;
    virtual void update(Worm& worm, sf::Time deltaTime) = 0;
    virtual void onExit(Worm& worm) = 0;
};