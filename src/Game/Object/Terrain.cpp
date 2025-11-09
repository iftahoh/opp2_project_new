// src/GameObjects/Terrain.cpp
#include "Game/Object/Terrain.h"
#include "Game/CollisionCategories.h"
#include <random>
#include <iostream>
#include <algorithm>

Terrain::Terrain(b2World& world, const sf::Vector2u& windowSize) {
    // ---- שינוי ----
    // נמחק את המשתנה המקומי std::vector<b2Vec2> terrainPhysicsVertices;
    // ונשתמש ב-m_surfacePoints במקום
    // ---------------
    const float screenWidth = windowSize.x;
    const float screenHeight = windowSize.y;
    const float stepX = 10.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(-4.0f, 6.0f);

    float currentY = 550.0f;

    // יצירת קו המתאר העליון
    for (float x = 0; x <= screenWidth; x += stepX) {
        // --- שימוש במשתנה החדש ---
        m_surfacePoints.push_back(b2Vec2(x / SCALE, currentY / SCALE)); //
        currentY += distr(gen);
        if (currentY < 450.0f) currentY = 450.0f; //
        if (currentY > 650.0f) currentY = 650.0f; //
    }

    // יצירת עותק לטובת סגירת הצורה (כדי שהמקור יישאר נקי)
    std::vector<b2Vec2> loopVertices = m_surfacePoints;
    loopVertices.push_back(b2Vec2(screenWidth / SCALE, screenHeight / SCALE)); //
    loopVertices.push_back(b2Vec2(0.0f, screenHeight / SCALE)); //

    // יצירת הגוף הפיזיקלי
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody; //
    groundBodyDef.position.Set(0.0f, 0.0f); //
    m_body = world.CreateBody(&groundBodyDef); //

    // ============================ התיקון הקריטי כאן ============================
    // אנחנו חייבים לקשר את המצביע של אובייקט ה-Terrain לגוף הפיזיקלי שלו,
    // כדי שנוכל לזהות אותו ב-ContactListener.
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this); //
    // ========================================================================

    b2ChainShape terrainShape;
    // --- שימוש בעותק ---
    terrainShape.CreateLoop(loopVertices.data(), loopVertices.size()); //

    b2FixtureDef terrainFixtureDef;
    terrainFixtureDef.shape = &terrainShape; //
    terrainFixtureDef.friction = 0.6f; //

    terrainFixtureDef.filter.categoryBits = CATEGORY_TERRAIN; //

    m_body->CreateFixture(&terrainFixtureDef); //

    // יצירת הייצוג הגרפי
    if (m_terrainTexture.loadFromFile("text.png")) { //
        m_terrainTexture.setRepeated(true); //
    }
    else {
        std::cerr << "Error loading terrain texture (text.png)" << std::endl; //
    }

    m_terrainDrawable.setPrimitiveType(sf::TrianglesStrip); //
    // --- שימוש במשתנה החדש ---
    for (size_t i = 0; i < m_surfacePoints.size(); ++i) { //
        b2Vec2 top_physics_pos = m_surfacePoints[i]; //
        sf::Vertex top_vertex;
        top_vertex.position = sf::Vector2f(top_physics_pos.x * SCALE, top_physics_pos.y * SCALE); //
        top_vertex.texCoords = sf::Vector2f(top_physics_pos.x * SCALE, 0); //
        top_vertex.color = sf::Color::White; //
        sf::Vertex bottom_vertex;
        bottom_vertex.position = sf::Vector2f(top_physics_pos.x * SCALE, screenHeight); //
        bottom_vertex.texCoords = sf::Vector2f(top_physics_pos.x * SCALE, m_terrainTexture.getSize().y); //
        bottom_vertex.color = sf::Color::White; //
        m_terrainDrawable.append(top_vertex); //
        m_terrainDrawable.append(bottom_vertex); //
    }
}

void Terrain::render(sf::RenderWindow& window) {
    if (m_terrainTexture.getSize().x > 0) {
        window.draw(m_terrainDrawable, &m_terrainTexture);
    }
    else {
        window.draw(m_terrainDrawable);
    }
}

float Terrain::getSurfaceY(float x) const {
    if (m_surfacePoints.empty()) {
        return 500.f; // ערך ברירת מחדל אם משהו השתבש
    }

    // 1. נמיר את ה-X מפיקסלים למטרי Box2D
    float x_meters = x / SCALE;

    // 2. נמצא את שתי הנקודות על פני השטח שה-X ביניהן
    // (אנו מניחים שהנקודות מסודרות לפי X)
    auto it = std::lower_bound(m_surfacePoints.begin(), m_surfacePoints.end(), x_meters,
        [](const b2Vec2& p, float x) { return p.x < x; });

    // 3. טיפול בקצוות
    if (it == m_surfacePoints.begin()) {
        return m_surfacePoints.front().y * SCALE; // X לפני ההתחלה
    }
    if (it == m_surfacePoints.end()) {
        return m_surfacePoints.back().y * SCALE; // X אחרי הסוף
    }

    // 4. קיבלנו שתי נקודות, נבצע חישוב יחס (אינטרפולציה)
    const b2Vec2& p2 = *it;
    const b2Vec2& p1 = *(--it); // הנקודה שלפני

    if (p2.x == p1.x) {
        return p1.y * SCALE; // מניעת חלוקה באפס
    }

    // 5. חישוב היחס
    float t = (x_meters - p1.x) / (p2.x - p1.x);

    // 6. חישוב ה-Y החדש (במטרים)
    float y_meters = p1.y + t * (p2.y - p1.y);

    // 7. החזרת הערך בפיקסלים
    return y_meters * SCALE;
}