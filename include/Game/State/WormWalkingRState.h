#pragma once
#include "Game/State/IWormState.h"

/*
WormWalkingRState class representing the walking right state of a worm.
*/

class WormWalkingRState : public IWormState {
public:
    void onEnter(Worm& worm) override;
    void handleInput(Worm& worm, const sf::Event& event) override;
    void update(Worm& worm, sf::Time deltaTime) override;
    void onExit(Worm& worm) override;
};