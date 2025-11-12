#include "Game/State/WormIdleState.h"
#include "Game/State/WormWalkingRState.h"
#include "Game/State/WormJumpState.h"
#include "Game/Object/Worm.h"

void WormIdleState::onEnter(Worm& worm) {
    worm.setAnimation("idle");
}

void WormIdleState::handleInput(Worm& worm, const sf::Event& event) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        worm.setState(std::make_unique<WormWalkingRState>());
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        if (worm.canJump()) {
            worm.setState(std::make_unique<WormJumpState>());
        }
    }
}

void WormIdleState::update(Worm& worm, sf::Time deltaTime) {
}

void WormIdleState::onExit(Worm& worm) {
}