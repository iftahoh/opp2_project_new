// include/GameObjects/Ball.h
#pragma once
#include "Game/Object/DynamicObject.h"

class Ball : public DynamicObject {
public:
    Ball(b2World& world, const sf::Vector2f& position);
    void render(sf::RenderWindow& window) override;

private:
    sf::CircleShape m_ballShape;
    static constexpr float BALL_RADIUS_PIXELS = 20.f;
    static constexpr float SCALE = 30.0f;
};