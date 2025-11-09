// include/Game/Object/Weapon/Projectile.h

#pragma once
#include "Game/Object/DynamicObject.h"

class Worm;
class GameController;

class Projectile : public DynamicObject {
public:
    Projectile(GameController& controller, b2World& world, sf::Vector2f pos,
        const std::string& textureKey, float radius);

  
    virtual void onCollision() = 0;

    bool isDead() const override { return m_isDead; }

    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;
    Worm* getOwner() const;

protected:
    Worm* m_owner; 
    GameController& m_controller;
    static constexpr float SCALE = 30.0f;
};