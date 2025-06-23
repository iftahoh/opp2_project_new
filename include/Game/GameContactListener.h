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

        // קבל את המצביעים לאובייקטי המשחק מהגופים הפיזיקליים
        auto objectA = reinterpret_cast<GameObject*>(fixtureA->GetBody()->GetUserData().pointer);
        auto objectB = reinterpret_cast<GameObject*>(fixtureB->GetBody()->GetUserData().pointer);

        if (!objectA || !objectB) {
            return;
        }

        // נסה להמיר את האובייקטים לתולעת ולקליע
        auto worm = dynamic_cast<Worm*>(objectA);
        auto projectile = dynamic_cast<Projectile*>(objectB);

        // אם ההמרה לא הצליחה, נסה הפוך
        if (!worm || !projectile) {
            worm = dynamic_cast<Worm*>(objectB);
            projectile = dynamic_cast<Projectile*>(objectA);
        }

        // אם יש לנו התנגשות בין תולעת לקליע
        if (worm && projectile) {
            // בדוק אם התולעת היא הבעלים של הקליע
            if (projectile->getOwner() == worm) {
                // אם כן, בטל את ההתנגשות הזו בלבד
                contact->SetEnabled(false);
            }
        }
    }
};