#include "Game/Object/Weapon/GrenadeShell.h"
#include "ResourceGraphic.h"
#include "Game/CollisionCategories.h"
#include "Game/Object/DynamicObject.h"
#include "Game/Object/Worm.h" // חשוב לוודא שקובץ זה כלול
#include <iostream>

// הגדרת רדיוס הפיצוץ במטרים של Box2D (שווה בערך ל-60 פיקסלים)
const float GrenadeShell::EXPLOSION_RADIUS = 2.0f;

// --- בנאי ---
// (הבנאי נשאר כפי שהיה אצלך, ללא שינוי)
GrenadeShell::GrenadeShell(b2World& world, const sf::Vector2f& position, Worm* owner)
    : Projectile(world, position, owner),
    m_explosionTexture(ResourceGraphic::getInstance().getTexture("firehit")),
    m_exploding(false)
{
    m_sprite.setTexture(ResourceGraphic::getInstance().getTexture("grenade"));
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
    m_sprite.setPosition(position);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);
    bodyDef.bullet = true;
    m_body = world.CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = (m_sprite.getTextureRect().height / 2.f) / SCALE;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;

    fixtureDef.filter.categoryBits = CATEGORY_PROJECTILE;
    fixtureDef.filter.maskBits = CATEGORY_TERRAIN | CATEGORY_WORM;

    m_body->CreateFixture(&fixtureDef);
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    m_explosionRect = sf::IntRect(0, 0, 100, 100);
}


// --- פונקציה שנקראת בעת התנגשות ---
void GrenadeShell::onCollision() {
    if (m_exploding) {
        return; // אם כבר מתפוצצים, לא לעשות כלום
    }
    
    m_exploding = true;
    m_explosionTimer = sf::Time::Zero;

    // --- לוגיקת הנזק החדשה ---
    b2World* world = m_body->GetWorld();
    b2Vec2 explosionCenter = m_body->GetPosition();

    // הגדרת התיבה התוחמת לחיפוש אובייקטים מסביב לפיצוץ
    b2AABB aabb;
    aabb.lowerBound = explosionCenter - b2Vec2(EXPLOSION_RADIUS, EXPLOSION_RADIUS);
    aabb.upperBound = explosionCenter + b2Vec2(EXPLOSION_RADIUS, EXPLOSION_RADIUS);

    // ניצור Callback (פונקציית משוב) שתופעל עבור כל גוף שנמצא בתיבה
    class ExplosionQueryCallback : public b2QueryCallback {
    public:
        ExplosionQueryCallback(const b2Vec2& center, float radius)
            : m_center(center), m_radius(radius) {
        }

        bool ReportFixture(b2Fixture* fixture) override {
            b2Body* body = fixture->GetBody();
            GameObject* gameObject = reinterpret_cast<GameObject*>(body->GetUserData().pointer);

            // נבדוק אם האובייקט הוא תולעת
            if (auto worm = dynamic_cast<Worm*>(gameObject)) {
                b2Vec2 wormPos = body->GetPosition();
                float distance = b2Distance(m_center, wormPos);

                // אם התולעת בתוך רדיוס הפיצוץ, היא נפגעת
                if (distance < m_radius) {
                    worm->takeDamage(GRENADE_DAMAGE);
                }
            }
            return true; // להמשיך לחפש עוד אובייקטים
        }
    private:
        b2Vec2 m_center;
        float m_radius;
    };

    ExplosionQueryCallback callback(explosionCenter, EXPLOSION_RADIUS);
    world->QueryAABB(&callback, aabb); // הרצת החיפוש בעולם הפיזיקלי
}


// --- פונקציית העדכון ---
void GrenadeShell::update(sf::Time deltaTime) {
    if (m_exploding) {
        // הפיכת הגוף לסטטי כדי שלא יזוז בזמן הפיצוץ
        if (m_body && m_body->GetType() == b2_dynamicBody) {
            m_body->SetLinearVelocity(b2Vec2(0, 0));
            m_body->SetType(b2_staticBody);
        }

        m_explosionTimer += deltaTime;
        int frame = static_cast<int>(m_explosionTimer.asSeconds() / 0.1f);

        if (frame >= 8) {
            m_isDead = true; // סימון להסרה
        }
        else {
            // עדכון אנימציית הפיצוץ
            m_explosionRect.left = frame * 100;
            m_sprite.setTexture(m_explosionTexture);
            m_sprite.setTextureRect(m_explosionRect);
            m_sprite.setOrigin(m_explosionRect.width / 2.f, m_explosionRect.height / 2.f);
        }
    }
    else {
        // עדכון רגיל של קליע בתנועה
        DynamicObject::update(deltaTime);
        if (m_body && m_body->GetLinearVelocity().LengthSquared() > 0) {
            float angle = atan2(m_body->GetLinearVelocity().y, m_body->GetLinearVelocity().x) * 180 / b2_pi;
            m_sprite.setRotation(angle);
        }
    }
}


// --- פונקציית הציור ---
// (הפונקציה נשארת כפי שהייתה אצלך, ללא שינוי)
void GrenadeShell::render(sf::RenderWindow& window) {
    window.draw(m_sprite);
}