// src/Game/State/WormJumpState.cpp
#include "Game/State/WormJumpState.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Worm.h"
#include <iostream>

namespace {
    const float JUMP_FORCE = 30.f;
}

void WormJumpState::onEnter(Worm& worm) {
    worm.setAnimation("jump");
    worm.applyForce(b2Vec2(0.0f, -JUMP_FORCE));
    worm.useJump(); // מקטינים את מונה הקפיצות
}

void WormJumpState::handleInput(Worm& worm, const sf::Event& event) {
    // במצב קפיצה, לא נאפשר קלט נוסף עד שהמצב יתחלף.
    // הלוגיקה הזו תעבור למצבים האחרים.
}

void WormJumpState::update(Worm& worm, sf::Time deltaTime) {
    // לאחר שהפעלנו את הכוח, נחזור מיד למצב עמידה.
    // זה יאפשר לקלוט את הלחיצה הבאה על רווח עבור קפיצה כפולה.
    worm.setState(std::make_unique<WormIdleState>());
}

void WormJumpState::onExit(Worm& worm) {
    // אין צורך בפעולה מיוחדת
}