#pragma once

#include "Game/Object/Weapon/Projectile.h"

class GrenadeShell : public Projectile {
public:
    GrenadeShell(GameController& controller, b2World& world, sf::Vector2f pos);
    void onCollision() override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    static const int GRENADE_DAMAGE = 20;
    static const float EXPLOSION_RADIUS;
    // ----------------------------

    bool m_exploding = false;
    
    sf::Texture& m_explosionTexture;
    sf::IntRect m_explosionRect;
    sf::Time m_explosionTimer;

    sf::Time m_lifetime;            // סופר כמה זמן עבר
    sf::Time m_detonationTime = sf::seconds(5); // הזמן עד לפיצוץ

};