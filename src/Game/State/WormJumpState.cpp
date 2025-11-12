#include "Game/State/WormJumpState.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Worm.h"
#include <iostream>

namespace {
    const float JUMP_FORCE = 7.f;
}

void WormJumpState::onEnter(Worm& worm) {
    worm.setAnimation("jump");
    worm.applyForce(b2Vec2(0.0f, -JUMP_FORCE));
    worm.useJump();
}

void WormJumpState::handleInput(Worm& worm, const sf::Event& event) {
}

void WormJumpState::update(Worm& worm, sf::Time deltaTime) {
    worm.setState(std::make_unique<WormIdleState>());
}

void WormJumpState::onExit(Worm& worm) {
}