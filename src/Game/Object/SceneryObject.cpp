#include "Game/Object/SceneryObject.h"
#include "ResourceGraphic.h" // נשתמש ב-ResourceGraphic לטעינת הטקסטורות
#include "Game/CollisionCategories.h" // אם נרצה קטגוריות
#include <iostream>

SceneryObject::SceneryObject(b2World& world, const sf::Vector2f& position, const std::string& textureKey)
{
    // 1. טעינת הטקסטורה והגדרת הספריט
    try {
        // אנחנו מניחים שהטקסטורות נטענו מראש לתוך ResourceGraphic
        // אם לא, נצטרך לטעון אותן ידנית (כמו ב-Terrain)
        m_texture = ResourceGraphic::getInstance().getTexture(textureKey);
        m_sprite.setTexture(m_texture);

        // מרכוז הספריט
        sf::FloatRect bounds = m_sprite.getLocalBounds();
        m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        m_sprite.setPosition(position);
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load texture for SceneryObject: " << textureKey << " (" << e.what() << ")\n";
        // ניצור צורת מלבן בסיסית במקום אם הטעינה נכשלה
        sf::RectangleShape* fallback = new sf::RectangleShape({ 32.f, 32.f });
        fallback->setFillColor(sf::Color::Magenta);
        fallback->setOrigin(16.f, 16.f);
        fallback->setPosition(position);
        // (הערה: זה ידלוף זיכרון, זה רק לדוגמה. עדיף לטפל בזה עם m_shape)
    }

    // 2. הגדרת הגוף הפיזי (Box2D)
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody; // אובייקט סטטי
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);
    m_body = world.CreateBody(&bodyDef);

    // 3. הגדרת הצורה הפיזיקלית (מלבן בגודל התמונה)
    sf::Vector2u textureSize = m_texture.getSize();
    float width = static_cast<float>(textureSize.x);
    float height = static_cast<float>(textureSize.y);

    b2PolygonShape boxShape;
    boxShape.SetAsBox((width / 2.f) / SCALE, (height / 2.f) / SCALE);

    // 4. הגדרת ה-Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.friction = 0.5f;
    // fixtureDef.filter.categoryBits = CATEGORY_TERRAIN; // אפשר לשייך לקטגוריית השטח

    m_body->CreateFixture(&fixtureDef);

    // קשור את הגוף למחלקה זו (כדי שנוכל לזהות אותו בהתנגשויות אם נצטרך)
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
}

void SceneryObject::render(sf::RenderWindow& window) {
    // עדכון המיקום הגרפי (למקרה שהגוף זז, למרות שהוא סטטי)
    const auto& pos = m_body->GetPosition();
    m_sprite.setPosition(pos.x * SCALE, pos.y * SCALE);

    window.draw(m_sprite);
}