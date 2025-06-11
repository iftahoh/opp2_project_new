// src/GameObjects/Terrain.cpp
#include "Game/Object/Terrain.h"
#include "Game/CollisionCategories.h"
#include <random>
#include <iostream>

Terrain::Terrain(b2World& world, const sf::Vector2u& windowSize) {
    std::vector<b2Vec2> terrainPhysicsVertices;
    const float screenWidth = windowSize.x;
    const float screenHeight = windowSize.y;
    const float stepX = 10.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(-2.0f, 2.0f);

    float currentY = 550.0f;

    // יצירת קו המתאר העליון
    for (float x = 0; x <= screenWidth; x += stepX) {
        terrainPhysicsVertices.push_back(b2Vec2(x / SCALE, currentY / SCALE));
        currentY += distr(gen);
        if (currentY < 450.0f) currentY = 450.0f;
        if (currentY > 650.0f) currentY = 650.0f;
    }

    // סגירת הצורה מלמטה
    terrainPhysicsVertices.push_back(b2Vec2(screenWidth / SCALE, screenHeight / SCALE));
    terrainPhysicsVertices.push_back(b2Vec2(0.0f, screenHeight / SCALE));

    // יצירת הגוף הפיזיקלי
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(0.0f, 0.0f);
    m_body = world.CreateBody(&groundBodyDef);

    b2ChainShape terrainShape;
    terrainShape.CreateLoop(terrainPhysicsVertices.data(), terrainPhysicsVertices.size());

    b2FixtureDef terrainFixtureDef;
    terrainFixtureDef.shape = &terrainShape;
    terrainFixtureDef.friction = 0.6f;

    // ================== הוספת הגדרות הסינון לאדמה ==================
// 1. קביעת הקטגוריה של האדמה
    terrainFixtureDef.filter.categoryBits = CATEGORY_TERRAIN;
    // 2. קביעת מסכה (עם מי היא מתנגשת) - ברירת המחדל היא הכל, וזה בסדר.
    // terrainFixtureDef.filter.maskBits = ...; 
    // ===============================================================
    m_body->CreateFixture(&terrainFixtureDef);

    // יצירת הייצוג הגרפי
    if (m_terrainTexture.loadFromFile("text.png")) {
        m_terrainTexture.setRepeated(true);
    }
    else {
        std::cerr << "Error loading terrain texture (text.png)" << std::endl;
    }

    m_terrainDrawable.setPrimitiveType(sf::TrianglesStrip);
    for (size_t i = 0; i < terrainPhysicsVertices.size() - 2; ++i) {
        b2Vec2 top_physics_pos = terrainPhysicsVertices[i];
        sf::Vertex top_vertex;
        top_vertex.position = sf::Vector2f(top_physics_pos.x * SCALE, top_physics_pos.y * SCALE);
        top_vertex.texCoords = sf::Vector2f(top_physics_pos.x * SCALE, 0);
        top_vertex.color = sf::Color::White;
        sf::Vertex bottom_vertex;
        bottom_vertex.position = sf::Vector2f(top_physics_pos.x * SCALE, screenHeight);
        bottom_vertex.texCoords = sf::Vector2f(top_physics_pos.x * SCALE, m_terrainTexture.getSize().y);
        bottom_vertex.color = sf::Color::White;
        m_terrainDrawable.append(top_vertex);
        m_terrainDrawable.append(bottom_vertex);
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