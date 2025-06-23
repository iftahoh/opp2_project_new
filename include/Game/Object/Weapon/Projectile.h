#pragma once
#include "Game/Object/DynamicObject.h"

class Worm;

class Projectile : public DynamicObject {
public:
    Projectile(b2World& world, const sf::Vector2f& position, Worm* owner);
    virtual void onCollision() = 0;

    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;
    Worm* getOwner() const;

protected:
	Worm* m_owner; // כדי לדעת מי יורה כעת כדי שהנזק לא ישפיע אליוֳ
    static constexpr float SCALE = 30.0f;
};