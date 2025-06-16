#include "Game/Object/Weapon/Projectile.h"
#include "Game/Object/DynamicObject.h"

Projectile::Projectile(b2World& world, const sf::Vector2f& position) {
    // Base constructor
}

void Projectile::update(sf::Time deltaTime) {
    DynamicObject::update(deltaTime);
}

void Projectile::render(sf::RenderWindow& window) {
    window.draw(m_sprite);
}