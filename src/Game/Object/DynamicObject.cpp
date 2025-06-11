#include "Game/Object/DynamicObject.h"

void DynamicObject::update(sf::Time deltaTime) {
    // אם אין גוף פיזיקלי, אין מה לעדכן
    if (!m_body) {
        return;
    }

    // סנכרון המיקום והזווית של הייצוג הגרפי עם הגוף הפיזיקלי
    const b2Vec2& physicsPos = m_body->GetPosition();
    const float physicsAngle = m_body->GetAngle();

    // המרה חזרה למערכת הקואורדינטות של SFML
    m_sprite.setPosition(physicsPos.x * 30.0f, physicsPos.y * 30.0f);
    m_sprite.setRotation(physicsAngle * 180.f / b2_pi); // המרה מרדיאנים למעלות
}