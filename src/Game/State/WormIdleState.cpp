#include "Game/State/WormIdleState.h"
#include "Game/State/WormWalkingRState.h"
#include "Game/State/WormJumpState.h"
#include "Game/Object/Worm.h"

void WormIdleState::onEnter(Worm& worm) {
    // קובעים את האנימציה המתאימה בכניסה למצב
    worm.setAnimation("idle");
}

void WormIdleState::handleInput(Worm& worm, const sf::Event& event) {
    // תזוזה עדיין יכולה להיות רציפה
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        worm.setState(std::make_unique<WormWalkingRState>());
    }

    // קפיצה תהיה מבוססת אירוע
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        if (worm.canJump()) { // בודקים אם מותר לקפוץ
            worm.setState(std::make_unique<WormJumpState>());
        }
    }
}

void WormIdleState::update(Worm& worm, sf::Time deltaTime) {
    // אין לוגיקה שרצה כל פריים במצב זה
}

void WormIdleState::onExit(Worm& worm) {
    // אין צורך בפעולה מיוחדת ביציאה
}