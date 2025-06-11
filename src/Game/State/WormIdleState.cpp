#include "Game/State/WormIdleState.h"
#include "Game/State/WormWalkingRState.h"
#include "Game/Object/Worm.h"

void WormIdleState::onEnter(Worm& worm) {
    // ������ �� �������� ������� ������ ����
    worm.setAnimation("idle");
}

void WormIdleState::handleInput(Worm& worm) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        // �� ����� ���� �� ��� �����, ������ ���� �����
        worm.setState(std::make_unique<WormWalkingRState>());
    }
}

void WormIdleState::update(Worm& worm, sf::Time deltaTime) {
    // ��� ������ ���� �� ����� ���� ��
}

void WormIdleState::onExit(Worm& worm) {
    // ��� ���� ������ ������ ������
}