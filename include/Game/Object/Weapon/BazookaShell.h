#pragma once
#include "Game/Object/Weapon/Projectile.h"
#include "Game/GameController.h"

class BazookaShell : public Projectile {
public:
    BazookaShell(GameController& controller, b2World& world, sf::Vector2f pos);
    void onCollision() override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    static const int BAZOOKA_DAMAGE = 30;
    static const float EXPLOSION_RADIUS;
    // ----------------------------

    bool m_exploding = false;
    sf::Texture& m_explosionTexture;
    sf::IntRect m_explosionRect;
    sf::Time m_explosionTimer;
};