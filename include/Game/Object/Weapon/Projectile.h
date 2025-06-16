#pragma once
#include "Game/Object/DynamicObject.h"

class Projectile : public DynamicObject {
public:
    Projectile(b2World& world, const sf::Vector2f& position);

    // כל קליע יצטרך לממש את ההתנהגות שלו כאשר הוא פוגע במשהו.
    virtual void onCollision() = 0;

    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

protected:
    sf::CircleShape m_shape; // רוב הקליעים יהיו עגולים
    static constexpr float SCALE = 30.0f;
};