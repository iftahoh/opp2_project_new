#pragma once
#include "Game/State/IWormState.h"

/*
WormGraveState class representing the state of a worm when it is in a grave (dead).
*/

class WormGraveState : public IWormState {
public:
    void onEnter(Worm& worm) override;
    void handleInput(Worm& worm, const sf::Event& event) override;
    void update(Worm& worm, sf::Time deltaTime) override;
    void onExit(Worm& worm) override;
};
