// include/Game/Object/Weapon/BazookaShell.h

#pragma once
#include "Game/Object/Weapon/Projectile.h"

class BazookaShell : public Projectile {
public:
    BazookaShell(b2World& world, const sf::Vector2f& position, Worm* owner);
    void onCollision() override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    // --- הגדרות חדשות לפיצוץ ---
    static const int BAZOOKA_DAMAGE = 30;
    static const float EXPLOSION_RADIUS; // הגדרת הרדיוס במטרים של Box2D
    // ----------------------------

    bool m_exploding = false;
    // שמור רפרנס לטקסטורה במקום אובייקט מלא
    sf::Texture& m_explosionTexture;
    sf::IntRect m_explosionRect;
    sf::Time m_explosionTimer;
};