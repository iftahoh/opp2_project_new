#include "Game/State/WormWalkingRState.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Worm.h"

namespace {
    // ������ ������ ����� ����� ������� �� Box2D
    const float WALK_SPEED = 1.0f;
}

void WormWalkingRState::onEnter(Worm& worm) {
    worm.setAnimation("walk");
}

void WormWalkingRState::handleInput(Worm& worm) {
    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        // ����� ������ ���, ������ ������ ����� �����
        worm.setHorizontalVelocity(-WALK_SPEED);
        worm.updateDirection(true);
        isMoving = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        // ������ ������ ����� �����
        worm.setHorizontalVelocity(WALK_SPEED);
        worm.updateDirection(false);
        isMoving = true;
    }

    if (!isMoving) {
        // �� ������ �� ���� �� ����, ������ ���� �����
        worm.setState(std::make_unique<WormIdleState>());
    }
}

void WormWalkingRState::update(Worm& worm, sf::Time deltaTime) {
    // ��� ���� ������� ������ ���
}

void WormWalkingRState::onExit(Worm& worm) {
    // ������ ���� �����, ������ �� ������ ������� �������
    worm.setHorizontalVelocity(0.0f);
}