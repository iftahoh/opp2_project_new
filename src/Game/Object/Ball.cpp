// src/GameObjects/Ball.cpp
#include "Game/Object/Ball.h"

Ball::Ball(b2World& world, const sf::Vector2f& position) {
    // הגדרת הגוף הפיזיקלי
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(position.x / SCALE, position.y / SCALE);
    m_body = world.CreateBody(&ballBodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = BALL_RADIUS_PIXELS / SCALE;

    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &circleShape;
    ballFixtureDef.density = 1.0f;
    ballFixtureDef.friction = 0.4f;
    ballFixtureDef.restitution = 0.75f;
    m_body->CreateFixture(&ballFixtureDef);

    // הגדרת הייצוג הגרפי
    m_ballShape.setRadius(BALL_RADIUS_PIXELS);
    m_ballShape.setFillColor(sf::Color::Red);
    m_ballShape.setOrigin(m_ballShape.getRadius(), m_ballShape.getRadius());

    // עדכון ראשוני של המיקום הגרפי (מתוך DynamicObject::update)
    const b2Vec2& physicsPos = m_body->GetPosition();
    m_sprite.setPosition(physicsPos.x * SCALE, physicsPos.y * SCALE);
    m_ballShape.setPosition(m_sprite.getPosition());
}

void Ball::render(sf::RenderWindow& window) {
    // בגלל ש-update של DynamicObject מעדכן את m_sprite,
    // נשתמש במיקום של m_sprite כדי לעדכן את m_ballShape
    m_ballShape.setPosition(m_sprite.getPosition());
    m_ballShape.setRotation(m_sprite.getRotation());
    window.draw(m_ballShape);
}