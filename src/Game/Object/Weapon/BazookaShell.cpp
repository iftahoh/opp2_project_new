// src/Game/Object/Weapon/BazookaShell.cpp

#include "Game/Object/Weapon/BazookaShell.h"
#include "ResourceGraphic.h"
#include "Game/CollisionCategories.h"
#include "Game/Object/DynamicObject.h"
#include "Game/Object/Worm.h" 
#include <iostream>


BazookaShell::BazookaShell(b2World& world, const sf::Vector2f& position, Worm* owner)
    : Projectile(world, position, owner),
    m_explosionTexture(ResourceGraphic::getInstance().getTexture("firehit"))
{
    m_sprite.setTexture(ResourceGraphic::getInstance().getTexture("missil"));
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);

 
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);
    bodyDef.bullet = true;
    m_body = world.CreateBody(&bodyDef);


    m_sprite.setPosition(position);
    // =====================================================================

    // --- äâãøú öåøú ääúðâùåú (Fixture) ---
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

    m_explosionRect = sf::IntRect(0, 0, 100, 100);
}

// --- ôåð÷öéä ùð÷øàú áòú äúðâùåú ---
void BazookaShell::onCollision() {
    if (!m_exploding) {
        m_exploding = true;
        m_explosionTimer = sf::Time::Zero;
    }
}

// --- ôåð÷öééú äòãëåï ùøöä áëì ôøééí ---
void BazookaShell::update(sf::Time deltaTime) {
    if (m_exploding) {
        if (m_body && m_body->GetType() == b2_dynamicBody) {
            m_body->SetLinearVelocity(b2Vec2(0, 0));
            m_body->SetType(b2_staticBody);
        }

        m_explosionTimer += deltaTime;
        int frame = static_cast<int>(m_explosionTimer.asSeconds() / 0.1f);

        if (frame >= 8) {
            m_isDead = true;
        }
        else {
            m_explosionRect.left = frame * 100;
            m_sprite.setTexture(m_explosionTexture);
            m_sprite.setTextureRect(m_explosionRect);
            m_sprite.setOrigin(m_explosionRect.width / 2.f, m_explosionRect.height / 2.f);
        }
    }
    else {
        DynamicObject::update(deltaTime);

        if (m_body && m_body->GetLinearVelocity().LengthSquared() > 0) {
            float angle = atan2(m_body->GetLinearVelocity().x, -m_body->GetLinearVelocity().y) * 180 / b2_pi;
            m_sprite.setRotation(angle);
        }
    }
}

// --- ôåð÷öééú äöéåø ---
void BazookaShell::render(sf::RenderWindow& window) {
    window.draw(m_sprite);
}