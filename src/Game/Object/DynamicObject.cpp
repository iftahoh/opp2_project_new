#include "Game/Object/DynamicObject.h"

void DynamicObject::update(sf::Time deltaTime) {
    if (!m_body) {
        return;
    }

    const b2Vec2& physicsPos = m_body->GetPosition();
    const float physicsAngle = m_body->GetAngle();

    m_sprite.setPosition(physicsPos.x * 30.0f, physicsPos.y * 30.0f);
    m_sprite.setRotation(physicsAngle * 180.f / b2_pi);
}