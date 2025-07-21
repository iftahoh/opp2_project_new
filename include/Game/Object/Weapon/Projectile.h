// include/Game/Object/Weapon/Projectile.h

#pragma once
#include "Game/Object/DynamicObject.h"

class Worm;

class Projectile : public DynamicObject {
public:
    Projectile(b2World& world, const sf::Vector2f& position, Worm* owner);

    // îúåãä åéøèåàìéú èäåøä ùúîåîù òì éãé ëì ñåâ ÷ìéò.
    // äéà ð÷øàú ëàùø ä÷ìéò ôåâò áîùäå.
    virtual void onCollision() = 0;

    // îéîåù ùì äîúåãä isDead îäîçì÷ä GameObject.
    bool isDead() const override { return m_isDead; }

    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;
    Worm* getOwner() const;

protected:
    Worm* m_owner; // ëãé ìãòú îé éåøä ëòú ëãé ùäðæ÷ ìà éùôéò àìéå

    // ãâì ùîöééï àí ä÷ìéò ñééí àú çééå åéù ìäñéø àåúå.
    
    static constexpr float SCALE = 30.0f;
};