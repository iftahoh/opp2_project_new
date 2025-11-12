#pragma once

#include "Game/State/IWormState.h"

/*
WormIdleState class representing the idle state of a worm.
*/

class WormIdleState : public IWormState {
public:
    void onEnter(Worm& worm) override;
    void handleInput(Worm& worm, const sf::Event& event) override;
    void update(Worm& worm, sf::Time deltaTime) override;
    void onExit(Worm& worm) override;
};