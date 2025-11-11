// src/Game/Object/Weapon/Bazooka.cpp

#include "Game/Object/Weapon/Bazooka.h"
#include "Game/Object/Worm.h"
#include "Game/GameController.h"
#include "Game/State/WormIdleState.h"
#include "Game/Object/Weapon/BazookaShell.h"
#include <cmath>
#include <iostream>

namespace {
    const float FIRE_POWER = 30.f;
    // äâãìðå àú äîøç÷ ääúçìúé ùì ä÷ìéò ì-40 ôé÷ñìéí (áî÷åí 30)
    const float SPAWN_OFFSET = 40.f;
}

Bazooka::Bazooka()
    : m_crosshairTexture(ResourceGraphic::getInstance().getTexture("crosshair")) {
    m_crosshairSprite.setTexture(m_crosshairTexture);
    m_crosshairSprite.setOrigin(m_crosshairTexture.getSize().x / 2.f, m_crosshairTexture.getSize().y / 2.f);

    const float desiredSize = 50.0f;
    sf::Vector2u textureSize = m_crosshairTexture.getSize();
    float scaleX = desiredSize / textureSize.x;
    float scaleY = desiredSize / textureSize.y;
    m_crosshairSprite.setScale(scaleX, scaleY);
    std::cout << "Bazooka equipped" << std::endl;
}

void Bazooka::handleInput(const sf::Event& event, Worm& worm) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            fire(worm);
            worm.setState(std::make_unique<WormIdleState>());
            // >>> סיום תור אחרי שימוש בנשק:
            //worm.getGameController().endTurn();
        }
    }
}

void Bazooka::update(Worm& worm, GameController& controller) {}

void Bazooka::render(sf::RenderWindow& window, const Worm& worm) {
    m_lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    m_crosshairSprite.setPosition(m_lastMousePos);
    window.draw(m_crosshairSprite);
}

void Bazooka::fire(Worm& worm) {
    GameController& controller = worm.getGameController();
    sf::Vector2f wormPos = worm.getSprite().getPosition();
    sf::Vector2f mousePos = m_lastMousePos;

    sf::Vector2f dir = mousePos - wormPos;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length == 0) return;
    dir /= length;

    // éåöøéí àú ä÷ìéò áîé÷åí çãù åîøåç÷ éåúø
    sf::Vector2f startPos = wormPos + dir * SPAWN_OFFSET;

    auto shell = std::make_unique<BazookaShell>(controller, controller.getWorld(), startPos);

    b2Vec2 impulse(dir.x * FIRE_POWER, dir.y * FIRE_POWER);
    shell->getBody()->ApplyLinearImpulseToCenter(impulse, true);

    controller.addGameObject(std::move(shell));
}

std::string Bazooka::getAimingAnimationName() const {
    return "bazooka_aim";
}

std::string Bazooka::getIdleAnimationName() const {
    return "bazooka_idle";
}
