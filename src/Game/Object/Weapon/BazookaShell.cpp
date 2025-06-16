#include "Game/Object/Weapon/BazookaShell.h"
#include "ResourceGraphic.h"
#include "Game/CollisionCategories.h"
#include <iostream>

BazookaShell::BazookaShell(b2World& world, const sf::Vector2f& position)
    : Projectile(world, position)
{
    m_sprite.setTexture(ResourceGraphic::getInstance().getTexture("missil"));
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);
    bodyDef.bullet = true;
    m_body = world.CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = (m_sprite.getTexture()->getSize().y / 2.f) / SCALE;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;

    fixtureDef.filter.categoryBits = CATEGORY_PROJECTILE;
    fixtureDef.filter.maskBits = CATEGORY_TERRAIN | CATEGORY_WORM;

    m_body->CreateFixture(&fixtureDef);
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
}

void BazookaShell::onCollision() {
    std::cout << "Bazooka Shell Hit!" << std::endl;
}