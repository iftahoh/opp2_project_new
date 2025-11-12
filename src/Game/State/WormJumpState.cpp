// src/Game/State/WormJumpState.cpp
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
    worm.useJump(); // î÷èéðéí àú îåðä ä÷ôéöåú
}

void WormJumpState::handleInput(Worm& worm, const sf::Event& event) {
    // áîöá ÷ôéöä, ìà ðàôùø ÷ìè ðåñó òã ùäîöá éúçìó.
    // äìåâé÷ä äæå úòáåø ìîöáéí äàçøéí.
}

void WormJumpState::update(Worm& worm, sf::Time deltaTime) {
    // ìàçø ùäôòìðå àú äëåç, ðçæåø îéã ìîöá òîéãä.
    // æä éàôùø ì÷ìåè àú äìçéöä äáàä òì øååç òáåø ÷ôéöä ëôåìä.
    worm.setState(std::make_unique<WormIdleState>());
}

void WormJumpState::onExit(Worm& worm) {
    // àéï öåøê áôòåìä îéåçãú
}