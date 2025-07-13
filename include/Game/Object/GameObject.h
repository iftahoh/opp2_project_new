// include/Game/Object/GameObject.h

#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class GameObject {
public:
    // וודא שההרסן קיים והוא וירטואלי
    virtual ~GameObject() = default;

    virtual void render(sf::RenderWindow& window) = 0;
    virtual void update(sf::Time deltaTime) = 0;

    // פונקציה וירטואלית שנוספה כדי לבדוק אם האובייקט "מת" ויש להסיר אותו.
    // ברירת המחדל היא false, ומחלקות יורשות יכולות לממש אותה אחרת.
    virtual bool isDead() const { return false; }

    b2Body* getBody() const { return m_body; }

protected:
    b2Body* m_body = nullptr;
    sf::Sprite m_sprite;
};