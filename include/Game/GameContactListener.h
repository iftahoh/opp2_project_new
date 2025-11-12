#pragma once

#include <box2d/box2d.h>
#include <iostream>
#include "Game/Object/GameObject.h"
#include "Game/Object/Worm.h"
#include "Game/Object/Weapon/Projectile.h"

/*
GameContactListener class that handles collision events in the game.
*/
class GameContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact) override
    {
        auto objectA = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        auto objectB = reinterpret_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

        if (!objectA || !objectB) return;

        Projectile* projectile = dynamic_cast<Projectile*>(objectA);
        GameObject* otherObject = objectB;

        if (!projectile) {
            projectile = dynamic_cast<Projectile*>(objectB);
            otherObject = objectA;
        }

        if (projectile) {
            Worm* ownerWorm = dynamic_cast<Worm*>(otherObject);
            if (ownerWorm && ownerWorm == projectile->getOwner()) {
                return;
            }

            projectile->onCollision();
        }
    }

    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        auto objectA = reinterpret_cast<GameObject*>(fixtureA->GetBody()->GetUserData().pointer);
        auto objectB = reinterpret_cast<GameObject*>(fixtureB->GetBody()->GetUserData().pointer);

        if (!objectA || !objectB) return;

        auto projectile = dynamic_cast<Projectile*>(objectA);
        auto worm = dynamic_cast<Worm*>(objectB);

        if (!projectile || !worm) {
            projectile = dynamic_cast<Projectile*>(objectB);
            worm = dynamic_cast<Worm*>(objectA);
        }

        if (worm && projectile) {
            if (projectile->getOwner() == worm) {
                contact->SetEnabled(false);
            }
        }
    }
};