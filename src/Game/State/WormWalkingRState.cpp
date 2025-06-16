#include "Game/State/WormWalkingRState.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Worm.h"
#include "Game/State/WormJumpState.h"

namespace {
    // קובעים מהירות הליכה קבועה ביחידות של Box2D
    const float WALK_SPEED = 3.f;
}

void WormWalkingRState::onEnter(Worm& worm) {
    worm.setAnimation("walk");
}

void WormWalkingRState::handleInput(Worm& worm, const sf::Event& event) {
    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        worm.setHorizontalVelocity(-WALK_SPEED);
        worm.updateDirection(true);
        isMoving = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        worm.setHorizontalVelocity(WALK_SPEED);
        worm.updateDirection(false);
        isMoving = true;
    }

    if (!isMoving) {
        worm.setState(std::make_unique<WormIdleState>());
        return; // יוצאים כדי לא לבדוק קפיצה אם כבר החלפנו מצב
    }

    // בדיקת קפיצה בזמן הליכה
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        if (worm.canJump()) {
            worm.setState(std::make_unique<WormJumpState>());
        }
    }
}

void WormWalkingRState::update(Worm& worm, sf::Time deltaTime) {
    // אין צורך בלוגיקה מיוחדת כאן
}

void WormWalkingRState::onExit(Worm& worm) {
    // ביציאה ממצב הליכה, עוצרים את התנועה האופקית לחלוטין
    worm.setHorizontalVelocity(0.0f);
}