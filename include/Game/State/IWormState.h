// include/Game/State/IWormState.h
#pragma once

#include <SFML/Graphics.hpp> // הוספת include
#include <SFML/System/Time.hpp>

class Worm;

class IWormState {
public:
    virtual ~IWormState() = default;
    virtual void onEnter(Worm& worm) = 0;
    // ----- שינוי חתימה -----
    virtual void handleInput(Worm& worm, const sf::Event& event) = 0;
    virtual void update(Worm& worm, sf::Time deltaTime) = 0;
    virtual void onExit(Worm& worm) = 0;
};