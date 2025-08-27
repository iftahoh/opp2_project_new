#pragma once
#include "Game/State/IWormState.h"

class WormGraveState : public IWormState {
public:
    void onEnter(Worm& worm) override;
    void handleInput(Worm& worm, const sf::Event& event) override;
    void update(Worm& worm, sf::Time deltaTime) override;
    void onExit(Worm& worm) override;
};
