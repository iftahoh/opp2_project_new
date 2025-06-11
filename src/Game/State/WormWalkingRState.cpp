#include "Game/State/WormWalkingRState.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Worm.h"

namespace {
    // קובעים מהירות הליכה קבועה ביחידות של Box2D
    const float WALK_SPEED = 1.0f;
}

void WormWalkingRState::onEnter(Worm& worm) {
    worm.setAnimation("walk");
}

void WormWalkingRState::handleInput(Worm& worm) {
    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        // במקום להפעיל כוח, קובעים מהירות קבועה שמאלה
        worm.setHorizontalVelocity(-WALK_SPEED);
        worm.updateDirection(true);
        isMoving = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        // קובעים מהירות קבועה ימינה
        worm.setHorizontalVelocity(WALK_SPEED);
        worm.updateDirection(false);
        isMoving = true;
    }

    if (!isMoving) {
        // אם המשתמש לא לוחץ על כלום, עוברים למצב עמידה
        worm.setState(std::make_unique<WormIdleState>());
    }
}

void WormWalkingRState::update(Worm& worm, sf::Time deltaTime) {
    // אין צורך בלוגיקה מיוחדת כאן
}

void WormWalkingRState::onExit(Worm& worm) {
    // ביציאה ממצב הליכה, עוצרים את התנועה האופקית לחלוטין
    worm.setHorizontalVelocity(0.0f);
}