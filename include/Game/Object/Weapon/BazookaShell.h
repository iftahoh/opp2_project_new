#pragma once
#include "Game/Object/Weapon/Projectile.h"

class BazookaShell : public Projectile {
public:
    BazookaShell(b2World& world, const sf::Vector2f& position);
    void onCollision() override;
};