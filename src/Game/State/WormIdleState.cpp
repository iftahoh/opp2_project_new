#include "Game/State/WormIdleState.h"
#include "Game/State/WormWalkingRState.h"
#include "Game/Object/Worm.h"

void WormIdleState::onEnter(Worm& worm) {
    // קובעים את האנימציה המתאימה בכניסה למצב
    worm.setAnimation("idle");
}

void WormIdleState::handleInput(Worm& worm) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        // אם השחקן לוחץ על מקש תזוזה, עוברים למצב הליכה
        worm.setState(std::make_unique<WormWalkingRState>());
    }
}

void WormIdleState::update(Worm& worm, sf::Time deltaTime) {
    // אין לוגיקה שרצה כל פריים במצב זה
}

void WormIdleState::onExit(Worm& worm) {
    // אין צורך בפעולה מיוחדת ביציאה
}