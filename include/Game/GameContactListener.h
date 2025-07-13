// include/Game/GameContactListener.h

#pragma once
#include <box2d/box2d.h>
#include <iostream>
#include "Game/Object/GameObject.h"
#include "Game/Object/Worm.h"
#include "Game/Object/Weapon/Projectile.h"

class GameContactListener : public b2ContactListener
{
public:
    // פונקציה זו נקראת ברגע שהתנגשות מתחילה
    void BeginContact(b2Contact* contact) override
    {
        // קבלת מצביעים לאובייקטים המעורבים בהתנגשות
        auto objectA = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        auto objectB = reinterpret_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

        if (!objectA || !objectB) return;

        // ======================= לוגיקה חדשה ומתוקנת =======================
        // נבדוק מי מהם הוא הקליע ומי האובייקט השני
        Projectile* projectile = dynamic_cast<Projectile*>(objectA);
        GameObject* otherObject = objectB;

        if (!projectile) {
            // אם אובייקט A אינו קליע, נבדוק אם אובייקט B הוא קליע
            projectile = dynamic_cast<Projectile*>(objectB);
            otherObject = objectA;
        }

        // אם מצאנו קליע בהתנגשות
        if (projectile) {
            // נבדוק אם האובייקט השני הוא התולעת שיצרה את הקליע
            Worm* ownerWorm = dynamic_cast<Worm*>(otherObject);
            if (ownerWorm && ownerWorm == projectile->getOwner()) {
                // אם כן, זו התנגשות עם היורה. אנחנו לא רוצים פיצוץ, אז פשוט נצא.
                return;
            }

            // אם זה כל אובייקט אחר (קרקע, תולעת אחרת וכו') - נפעיל את הפיצוץ.
            projectile->onCollision();
        }
        // ====================================================================
    }

    // PreSolve נשאר זהה - הוא מונע מהקליע להעיף את התולעת שיורה אותו
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