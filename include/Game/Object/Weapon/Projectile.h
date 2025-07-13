// include/Game/Object/Weapon/Projectile.h

#pragma once
#include "Game/Object/DynamicObject.h"

class Worm;

class Projectile : public DynamicObject {
public:
    Projectile(b2World& world, const sf::Vector2f& position, Worm* owner);

    // מתודה וירטואלית טהורה שתמומש על ידי כל סוג קליע.
    // היא נקראת כאשר הקליע פוגע במשהו.
    virtual void onCollision() = 0;

    // מימוש של המתודה isDead מהמחלקה GameObject.
    bool isDead() const override { return m_isDead; }

    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;
    Worm* getOwner() const;

protected:
    Worm* m_owner; // כדי לדעת מי יורה כעת כדי שהנזק לא ישפיע אליו

    // דגל שמציין אם הקליע סיים את חייו ויש להסיר אותו.
    bool m_isDead = false;
    static constexpr float SCALE = 30.0f;
};