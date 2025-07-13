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
    bool m_exploding = false;
    // שמור רפרנס לטקסטורה במקום אובייקט מלא
    sf::Texture& m_explosionTexture;
    sf::IntRect m_explosionRect;
    sf::Time m_explosionTimer;
};