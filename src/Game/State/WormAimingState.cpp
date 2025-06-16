#include "Game/State/WormAimingState.h"
#include "Game/Object/Worm.h"

WormAimingState::WormAimingState(std::unique_ptr<IWeapon> weapon)
    : m_weapon(std::move(weapon)) {
}

void WormAimingState::onEnter(Worm& worm) {
    worm.setAnimation("bazooka_aim");
}

void WormAimingState::handleInput(Worm& worm, const sf::Event& event) {
    if (m_weapon) {
        m_weapon->handleInput(event, worm);
    }
}

void WormAimingState::update(Worm& worm, sf::Time deltaTime) {
    // No update logic for now
}

void WormAimingState::render(sf::RenderWindow& window, Worm& worm) {
    if (m_weapon) {
        m_weapon->render(window, worm);
    }
}

void WormAimingState::onExit(Worm& worm) {
    worm.setAnimation("bazooka_idle");
}