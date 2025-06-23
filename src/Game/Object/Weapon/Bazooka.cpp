#include "Game/Object/Weapon/Bazooka.h"
#include "Game/Object/Worm.h"
#include "Game/GameController.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Weapon/BazookaShell.h"
#include <cmath>
#include <iostream>

namespace {
    const float FIRE_POWER = 15.f;
}

Bazooka::Bazooka()
    // צורה של כוונות במיקום של העכבר
    : m_crosshairTexture(ResourceGraphic::getInstance().getTexture("crosshair")) {
    m_crosshairSprite.setTexture(m_crosshairTexture);
    m_crosshairSprite.setOrigin(m_crosshairTexture.getSize().x / 2.f, m_crosshairTexture.getSize().y / 2.f);

    // התאמת הכוונות מבחינת פיקסלים
    const float desiredSize = 50.0f;
    sf::Vector2u textureSize = m_crosshairTexture.getSize();
    float scaleX = desiredSize / textureSize.x;
    float scaleY = desiredSize / textureSize.y;
    m_crosshairSprite.setScale(scaleX, scaleY);
}

void Bazooka::handleInput(const sf::Event& event, Worm& worm) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            fire(worm);
            worm.setState(std::make_unique<WormIdleState>());
        }
    }
}

void Bazooka::update(Worm& worm, GameController& controller) {}

void Bazooka::render(sf::RenderWindow& window, const Worm& worm) {
    m_lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    m_crosshairSprite.setPosition(m_lastMousePos); // השתמש במיקום המעודכן
    window.draw(m_crosshairSprite);
}

void Bazooka::fire(Worm& worm) {
    // ================== קוד אבחון - התחל ==================
    std::cout << "--- Firing Bazooka ---" << std::endl;
    sf::Vector2f wormPos = worm.getSprite().getPosition();
    sf::Vector2f mousePos = m_lastMousePos;
    std::cout << "Worm Position: (" << wormPos.x << ", " << wormPos.y << ")" << std::endl;
    std::cout << "Mouse (Target) Position: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
    // ================== קוד אבחון - סוף ===================
    GameController& controller = worm.getGameController();
  

    sf::Vector2f dir = mousePos - wormPos;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length == 0) return;
    dir /= length;

    sf::Vector2f startPos = wormPos + dir * 30.f;

    auto shell = std::make_unique<BazookaShell>(controller.getWorld(), startPos, &worm);


    b2Vec2 impulse(dir.x * FIRE_POWER, dir.y * FIRE_POWER);
    shell->getBody()->ApplyLinearImpulseToCenter(impulse, true);

    controller.addGameObject(std::move(shell));
}