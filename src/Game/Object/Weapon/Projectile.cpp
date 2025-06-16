#include "Game/Object/Weapon/Projectile.h"

Projectile::Projectile(b2World& world, const sf::Vector2f& position) {
    // בנאי ריק כרגע, המימוש יהיה במחלקות היורשות
}

void Projectile::update(sf::Time deltaTime) {
    DynamicObject::update(deltaTime);
    m_shape.setPosition(m_sprite.getPosition());
}

void Projectile::render(sf::RenderWindow& window) {
    window.draw(m_shape);
}