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
    // --- äâãøåú çãùåú ìôéöåõ ---
    static const int BAZOOKA_DAMAGE = 30;
    static const float EXPLOSION_RADIUS; // äâãøú äøãéåñ áîèøéí ùì Box2D
    // ----------------------------

    bool m_exploding = false;
    // ùîåø øôøðñ ìè÷ñèåøä áî÷åí àåáéé÷è îìà
    sf::Texture& m_explosionTexture;
    sf::IntRect m_explosionRect;
    sf::Time m_explosionTimer;
};