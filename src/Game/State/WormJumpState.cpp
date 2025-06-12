#include "Game/State/WormJumpState.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Worm.h"
#include <iostream>

namespace {
    // ������ ������ ����� ����� ������� �� Box2D
    const float JUMP_FORCE = 5.f;
}

void WormJumpState::onEnter(Worm& worm) {
    worm.setAnimation("jump");
}

void WormJumpState::handleInput(Worm& worm) {
    bool isJumping = false;

	// ������ ������ ����� ������� �� Box2D
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		std::cout << "Jumping!" << std::endl;
		// ��� ���� ������� ������ ���
	    worm.applyForce(b2Vec2(0.0f, -JUMP_FORCE));
        isJumping = true;
    }

    if(!isJumping) {
        // ��� ���� ������� ������ ���
        worm.setState(std::make_unique<WormIdleState>());
    }  
}

void WormJumpState::update(Worm& worm, sf::Time deltaTime) {
    // ��� ���� ������� ������ ���
}

void WormJumpState::onExit(Worm& worm) {
    // ������ ���� �����, ������ �� ������ ������� �������
    worm.setHorizontalVelocity(0.0f);
}