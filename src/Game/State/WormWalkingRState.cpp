#include "Game/State/WormWalkingRState.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Worm.h"
#include "Game/State/WormJumpState.h"

namespace {
    // ÷åáòéí îäéøåú äìéëä ÷áåòä áéçéãåú ùì Box2D
    const float WALK_SPEED = 2.f;
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
        return; // éåöàéí ëãé ìà ìáãå÷ ÷ôéöä àí ëáø äçìôðå îöá
    }

    // áãé÷ú ÷ôéöä áæîï äìéëä
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        if (worm.canJump()) {
            worm.setState(std::make_unique<WormJumpState>());
        }
    }
}

void WormWalkingRState::update(Worm& worm, sf::Time deltaTime) {
    // àéï öåøê áìåâé÷ä îéåçãú ëàï
}

void WormWalkingRState::onExit(Worm& worm) {
    // áéöéàä îîöá äìéëä, òåöøéí àú äúðåòä äàåô÷éú ìçìåèéï
    worm.setHorizontalVelocity(0.0f);
}