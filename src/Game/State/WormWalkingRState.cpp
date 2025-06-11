#include "Game/State/WormWalkingRState.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Worm.h"

void WormWalkingRState::onEnter(Worm& worm) {
    worm.setAnimation("walk");
}

void WormWalkingRState::handleInput(Worm& worm) {
    bool isMoving = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        worm.applyForce(b2Vec2(-20.0f, 0.0f));
        worm.updateDirection(true);
        isMoving = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        worm.applyForce(b2Vec2(20.0f, 0.0f));
        worm.updateDirection(false);
        isMoving = true;
    }

    if (!isMoving) {
        worm.setState(std::make_unique<WormIdleState>());
    }
}

void WormWalkingRState::update(Worm& worm, sf::Time deltaTime) {}

void WormWalkingRState::onExit(Worm& worm) {
    b2Vec2 currentVel = worm.getBody()->GetLinearVelocity();
    currentVel.x = 0;
    worm.getBody()->SetLinearVelocity(currentVel);
}