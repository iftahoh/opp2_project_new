#pragma once
#include <box2d/box2d.h>
#include <iostream>
#include "Game/Object/GameObject.h"
#include "Game/Object/Worm.h"
#include "Game/Object/Weapon/Projectile.h"

class GameContactListener : public b2ContactListener
{
public:
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // ��� �� �������� ��������� ����� ������� ����������
        auto objectA = reinterpret_cast<GameObject*>(fixtureA->GetBody()->GetUserData().pointer);
        auto objectB = reinterpret_cast<GameObject*>(fixtureB->GetBody()->GetUserData().pointer);

        if (!objectA || !objectB) {
            return;
        }

        // ��� ����� �� ���������� ������ ������
        auto worm = dynamic_cast<Worm*>(objectA);
        auto projectile = dynamic_cast<Projectile*>(objectB);

        // �� ����� �� ������, ��� ����
        if (!worm || !projectile) {
            worm = dynamic_cast<Worm*>(objectB);
            projectile = dynamic_cast<Projectile*>(objectA);
        }

        // �� �� ��� ������� ��� ����� �����
        if (worm && projectile) {
            // ���� �� ������ ��� ������ �� �����
            if (projectile->getOwner() == worm) {
                // �� ��, ��� �� �������� ��� ����
                contact->SetEnabled(false);
            }
        }
    }
};