#include "Game/Player.h"
#include "Game/GameController.h"
#include "Game/Object/Weapon/Bazooka.h"
#include <iostream>

const int NUM_WORMS_PER_PLAYER = 5;

Player::Player(b2World& world, GameController& gameController, const sf::Vector2f& basePosition)
    : m_currentWormIndex(0), m_gameController(gameController) {

    for (int i = 0; i < NUM_WORMS_PER_PLAYER; ++i) {
        sf::Vector2f wormPos = basePosition + sf::Vector2f(i * 50.f, 0.f);

        const float y_offset = 3.0f * b2_linearSlop * 30.0f;
        wormPos.y += y_offset;

        auto worm = std::make_unique<Worm>(world, m_gameController, wormPos);

        m_worms.push_back(worm.get());
        m_gameController.addGameObject(std::move(worm));
    }
}

void Player::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Tab) {
            nextWorm();
            return;
        }
        if (event.key.code == sf::Keyboard::Num1) {
            if (!m_worms.empty()) {
                m_worms[m_currentWormIndex]->equipWeapon(std::make_unique<Bazooka>());
                return;
            }
        }
    }

    if (!m_worms.empty()) {
        m_worms[m_currentWormIndex]->handlePlayerInput(event);
    }
}

void Player::nextWorm() {
    if (m_worms.empty()) return;
    m_worms[m_currentWormIndex]->getBody()->SetLinearVelocity(b2Vec2(0, 0));
    m_currentWormIndex = (m_currentWormIndex + 1) % m_worms.size();
    std::cout << "Switched to worm " << m_currentWormIndex + 1 << std::endl;
}

void Player::update(sf::Time deltaTime) {
}

void Player::render(sf::RenderWindow& window) {
}

sf::Vector2f Player::getPosition() const {
	b2Vec2 pos = m_worms[m_currentWormIndex]->getBody()->GetPosition();
	return sf::Vector2f(pos.x * GameController::SCALE, pos.y * GameController::SCALE);
}