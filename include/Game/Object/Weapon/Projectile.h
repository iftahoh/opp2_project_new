// include/Game/Object/Weapon/Projectile.h

#pragma once
#include "Game/Object/DynamicObject.h"

class Worm;

class Projectile : public DynamicObject {
public:
    Projectile(b2World& world, const sf::Vector2f& position, Worm* owner);

    // ����� ��������� ����� ������ �� ��� �� ��� ����.
    // ��� ����� ���� ����� ���� �����.
    virtual void onCollision() = 0;

    // ����� �� ������ isDead ������� GameObject.
    bool isDead() const override { return m_isDead; }

    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;
    Worm* getOwner() const;

protected:
    Worm* m_owner; // ��� ���� �� ���� ��� ��� ����� �� ����� ����

    // ��� ������ �� ����� ���� �� ���� ��� ����� ����.
    bool m_isDead = false;
    static constexpr float SCALE = 30.0f;
};