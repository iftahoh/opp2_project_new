#include "Game/State/WormAimingState.h"
#include "Game/Object/Worm.h"
#include "Game/GameController.h" // ����� ������ ���� ���� �����

WormAimingState::WormAimingState(std::unique_ptr<IWeapon> weapon)
    : m_weapon(std::move(weapon)) {
}

void WormAimingState::onEnter(Worm& worm) {
    // ���� ���� ����� �� �������� �� ������ ���� "�����"
    // worm.setAnimation("aiming");
}

void WormAimingState::handleInput(Worm& worm, const sf::Event& event) {
    if (m_weapon) {
        // ���� �� ������ ���� ���� ����
        m_weapon->handleInput(event, worm);
    }
}

void WormAimingState::update(Worm& worm, sf::Time deltaTime) {
    // ���� �� ���� (����, �� ����� ������)
    // ����� ��� ����� �� ��� ����� ��� ������ ���� ����
    // worm.getGameController() - ������� ����� ������
    // if (m_weapon) {
    //     m_weapon->update(worm, worm.getGameController());
    // }
}

void WormAimingState::onExit(Worm& worm) {
    // ������ ������ �������� ����� ���� ����� (����, ���� �����)
}