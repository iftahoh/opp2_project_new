#include "Game/State/WormJumpState.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Worm.h"
#include <iostream>

namespace {
    // קובעים מהירות הליכה קבועה ביחידות של Box2D
    const float JUMP_FORCE = 5.f;
}

void WormJumpState::onEnter(Worm& worm) {
    worm.setAnimation("jump");
}

void WormJumpState::handleInput(Worm& worm) {
    bool isJumping = false;

	// קובעים מהירות הליכה ביחידות של Box2D
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		std::cout << "Jumping!" << std::endl;
		// אין צורך בלוגיקה מיוחדת כאן
	    worm.applyForce(b2Vec2(0.0f, -JUMP_FORCE));
        isJumping = true;
    }

    if(!isJumping) {
        // אין צורך בלוגיקה מיוחדת כאן
        worm.setState(std::make_unique<WormIdleState>());
    }  
}

void WormJumpState::update(Worm& worm, sf::Time deltaTime) {
    // אין צורך בלוגיקה מיוחדת כאן
}

void WormJumpState::onExit(Worm& worm) {
    // ביציאה ממצב הליכה, עוצרים את התנועה האופקית לחלוטין
    worm.setHorizontalVelocity(0.0f);
}