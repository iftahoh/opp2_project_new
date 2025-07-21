// include/Game/Object/GameObject.h

#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class GameObject {
public:
    // ååãà ùääøñï ÷ééí åäåà åéøèåàìé
    virtual ~GameObject() = default;

    virtual void render(sf::RenderWindow& window) = 0;
    virtual void update(sf::Time deltaTime) = 0;

    // ôåð÷öéä åéøèåàìéú ùðåñôä ëãé ìáãå÷ àí äàåáéé÷è "îú" åéù ìäñéø àåúå.
    // áøéøú äîçãì äéà false, åîçì÷åú éåøùåú éëåìåú ìîîù àåúä àçøú.
    virtual bool isDead() const { return false; }

    b2Body* getBody() const { return m_body; }

protected:
    b2Body* m_body = nullptr;
    sf::Sprite m_sprite;
    bool m_isDead = false;
};