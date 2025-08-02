// include/Game/Object/Weapon/BazookaShell.h

#pragma once
#include "Game/Object/Weapon/Projectile.h"

class GrenadeShell : public Projectile {
public:
    GrenadeShell(b2World& world, const sf::Vector2f& position, Worm* owner);
    void onCollision() override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    // --- äâãøåú çãùåú ìôéöåõ ---
    static const int GRENADE_DAMAGE = 20;
    static const float EXPLOSION_RADIUS; // äâãøú äøãéåñ áîèøéí ùì Box2D
    // ----------------------------

    bool m_exploding = false;
    // ùîåø øôøðñ ìè÷ñèåøä áî÷åí àåáéé÷è îìà
    sf::Texture& m_explosionTexture;
    sf::IntRect m_explosionRect;
    sf::Time m_explosionTimer;

    sf::Time m_lifetime;            // סופר כמה זמן עבר
    sf::Time m_detonationTime = sf::seconds(5); // הזמן עד לפיצוץ

};