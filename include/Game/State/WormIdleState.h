#pragma once
#include "Game/State/IWormState.h"

class WormIdleState : public IWormState {
public:
    void onEnter(Worm& worm) override;
    void handleInput(Worm& worm) override;
    void update(Worm& worm, sf::Time deltaTime) override;
    void onExit(Worm& worm) override;
};