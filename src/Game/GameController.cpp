#include "Game/GameController.h"
#include <iostream>
#include <random>

GameController::GameController(sf::RenderWindow& window)
    : m_window(window), m_world(b2Vec2(0.0f, 9.8f)), m_ballBody(nullptr) // אתחול m_ballBody
{
    setupWorld();
    setupGraphics();
}

void GameController::setupWorld() {
    m_terrainPhysicsVertices.clear();

    const float screenWidth = m_window.getSize().x;
    const float screenHeight = m_window.getSize().y;
    const float stepX = 10.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(-2.0f, 2.0f);

    // === שינוי 1: גובה התחלתי נמוך יותר ===
    float currentY = 550.0f;

    // יצירת קו המתאר העליון
    for (float x = 0; x <= screenWidth; x += stepX) {
        m_terrainPhysicsVertices.push_back(b2Vec2(x / SCALE, currentY / SCALE));
        currentY += distr(gen);

        // === שינוי 2: עדכון טווח הגבהים המותר ===
        if (currentY < 450.0f) currentY = 450.0f;
        if (currentY > 650.0f) currentY = 650.0f;
    }

    // סגירת הצורה מלמטה (נשאר אותו הדבר)
    m_terrainPhysicsVertices.push_back(b2Vec2(screenWidth / SCALE, screenHeight / SCALE));
    m_terrainPhysicsVertices.push_back(b2Vec2(0.0f, screenHeight / SCALE));

    // יצירת הגוף הפיזיקלי (נשאר אותו הדבר)
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(0.0f, 0.0f);

    b2Body* groundBody = m_world.CreateBody(&groundBodyDef);

    b2ChainShape terrainShape;
    terrainShape.CreateLoop(m_terrainPhysicsVertices.data(), m_terrainPhysicsVertices.size());

    b2FixtureDef terrainFixtureDef;
    terrainFixtureDef.shape = &terrainShape;
    terrainFixtureDef.density = 0.0f;
    terrainFixtureDef.friction = 0.6f;

    groundBody->CreateFixture(&terrainFixtureDef);

    // --- יצירת הכדור (נשאר אותו הדבר) ---
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set((screenWidth / 2.f) / SCALE, 100.f / SCALE);
    m_ballBody = m_world.CreateBody(&ballBodyDef);
    b2CircleShape circleShape;
    circleShape.m_radius = 20.f / SCALE;
    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &circleShape;
    ballFixtureDef.density = 1.0f;
    ballFixtureDef.friction = 0.4f;
    ballFixtureDef.restitution = 0.75f;
    m_ballBody->CreateFixture(&ballFixtureDef);
}

void GameController::setupGraphics() {
    // === הוספת לוגיקה לטעינת רקע השמיים ===
    // ודא שיש לך קובץ בשם sky_texture.png בתיקיית המשאבים שלך
    if (!m_skyTexture.loadFromFile("beach_background.png")) {
        // טיפול בשגיאה אם הקובץ לא נמצא
        std::cerr << "Error loading sky texture" << std::endl;
    }
    m_skySprite.setTexture(m_skyTexture);

    // התאמת גודל התמונה לגודל החלון
    sf::Vector2u windowSize = m_window.getSize();
    sf::Vector2u textureSize = m_skyTexture.getSize();
    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    m_skySprite.setScale(scaleX, scaleY);
    // --- קוד הגדרת גרפיקת השטח נשאר כפי שהיה ---
    if (m_terrainTexture.loadFromFile("text.png")) {
        m_terrainTexture.setRepeated(true);
    }
    m_terrainDrawable.clear();
    m_terrainDrawable.setPrimitiveType(sf::TrianglesStrip);
    const float screenHeight = m_window.getSize().y;
    for (size_t i = 0; i < m_terrainPhysicsVertices.size() - 2; ++i) {
        b2Vec2 top_physics_pos = m_terrainPhysicsVertices[i];
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
    // --- סוף קוד גרפיקת השטח ---


    // === הוספת הגדרות גרפיות לכדור ===
    m_ballShape.setRadius(20.f); // רדיוס בפיקסלים, זהה למה שהגדרנו לפיזיקה
    m_ballShape.setFillColor(sf::Color::Red);
    m_ballShape.setOrigin(m_ballShape.getRadius(), m_ballShape.getRadius()); // מרכוז הצורה לציור קל
}

void GameController::update(sf::Time deltaTime) {
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    m_world.Step(deltaTime.asSeconds(), velocityIterations, positionIterations);

    // === הוספת עדכון מיקום הכדור הגרפי ===
    if (m_ballBody) {
        b2Vec2 physicsPos = m_ballBody->GetPosition();
        sf::Vector2f graphicsPos(physicsPos.x * SCALE, physicsPos.y * SCALE);
        m_ballShape.setPosition(graphicsPos);
    }
}

void GameController::render() {
    m_window.clear(sf::Color(135, 206, 235)); // Sky blue

    m_window.draw(m_skySprite);

    // ציור השטח
    if (m_terrainTexture.getSize().x > 0) {
        m_window.draw(m_terrainDrawable, &m_terrainTexture);
    }
    else {
        m_window.draw(m_terrainDrawable);
    }

    // === הוספת ציור הכדור ===
    m_window.draw(m_ballShape);

    m_window.display();
}


void GameController::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        handleEvents();

        sf::Time deltaTime = clock.restart();
        update(deltaTime);

        render();
    }
}

void GameController::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
    }
}


