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
    // --- ������ ����� ������ ---
    static const int BAZOOKA_DAMAGE = 30;
    static const float EXPLOSION_RADIUS; // ����� ������ ������ �� Box2D
    // ----------------------------

    bool m_exploding = false;
    // ���� ����� �������� ����� ������� ���
    sf::Texture& m_explosionTexture;
    sf::IntRect m_explosionRect;
    sf::Time m_explosionTimer;
};