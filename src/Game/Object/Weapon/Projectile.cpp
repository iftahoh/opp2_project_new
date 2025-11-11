#include "Game/Object/Weapon/Projectile.h"
#include "Game/Object/DynamicObject.h"
#include "Game/GameController.h"

Projectile::Projectile(GameController& controller, b2World& world, sf::Vector2f pos,
    const std::string& textureKey, float radius)
    : m_controller(controller), m_lifeTimer(5.f) {
    // Base constructor
}

void Projectile::update(sf::Time deltaTime) {
    DynamicObject::update(deltaTime);
}

void Projectile::render(sf::RenderWindow& window) {
    window.draw(m_sprite);
}

void Projectile::hit() {
    // הוספת בדיקה למניעת קריאות כפולות (מומלץ)
    if (m_isDead) return;

    m_isDead = true; //

    // --- הוסף את השורה הבאה ---
    // זה יסמן לבקר המשחק לסיים את התור בפריים הבא
    m_controller.requestEndTurn();
}

Worm* Projectile::getOwner() const
{
    return m_owner;
}
