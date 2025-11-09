#include "Game/GameController.h"
#include "Game/Object/Terrain.h"
#include "Game/Player.h"
#include "ResourceGraphic.h"                 // NEW: כדי להשתמש בפונט הטעון
#include <iostream>

// בקובץ GameController.cpp
GameController::GameController(sf::RenderWindow& window)
    : m_window(window),
    m_world(b2Vec2(0.0f, 9.8f)), // m_world הוא אכן ה-b2World, מצוין!
    m_cameraView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)),
    m_currentPlayerIndex(0)
{
    m_world.SetContactListener(&m_contactListener);
    setupWorld(); // הפונקציה הזו יוצרת את הטריין ושאר האובייקטים
    sf::Vector2u windowSize = m_window.getSize();

    // ===================================================================
    // קוד חדש ומתוקן שנוסף לבנאי
    // ===================================================================

    // 3. ניצור את הקירות הפיזיים באמצעות m_world ישירות
    sf::Vector2f mapSizePixels(windowSize.x * 4, windowSize.y);
    const float PIXELS_PER_METER = 30.0f; // ודא שזה יחס ההמרה שלך
    const float mapWidth = mapSizePixels.x / PIXELS_PER_METER;
    const float mapHeight = mapSizePixels.y / PIXELS_PER_METER;

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f);
    b2Body* groundBody = m_world.CreateBody(&groundBodyDef); // שימוש ישיר ב-m_world
    b2EdgeShape edgeShape;

    // הגדרת ארבעת הקירות
    edgeShape.SetTwoSided(b2Vec2(0.0f, mapHeight), b2Vec2(mapWidth, mapHeight)); // תחתון
    groundBody->CreateFixture(&edgeShape, 0.0f);
    edgeShape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(mapWidth, 0.0f)); // עליון
    groundBody->CreateFixture(&edgeShape, 0.0f);
    edgeShape.SetTwoSided(b2Vec2(0.0f, mapHeight), b2Vec2(0.0f, 0.0f)); // שמאלי
    groundBody->CreateFixture(&edgeShape, 0.0f);
    edgeShape.SetTwoSided(b2Vec2(mapWidth, mapHeight), b2Vec2(mapWidth, 0.0f)); // ימני
    groundBody->CreateFixture(&edgeShape, 0.0f);
}

GameController::~GameController() = default;

void GameController::addGameObject(std::unique_ptr<GameObject> object) {
    m_gameObjects.push_back(std::move(object));
}

b2World& GameController::getWorld() {
    return m_world;
}

void GameController::setupWorld() {
    if (!m_skyTexture.loadFromFile("beach_background.png")) {
        std::cerr << "Error loading sky texture" << std::endl;
    }
    m_skySprite.setTexture(m_skyTexture);
    sf::Vector2u windowSize = m_window.getSize();
    sf::Vector2u textureSize = m_skyTexture.getSize();
    m_skySprite.setScale((float)(windowSize.x / textureSize.x), (float)windowSize.y / textureSize.y);

    // --- התיקון ---
    // 1. הגדר את גודל עולם המשחק הרצוי. בוא ניצור עולם שרוחבו פי 4 מהחלון.
    sf::Vector2u worldSize(windowSize.x * 4, windowSize.y);

    // 2. השתמש בגודל העולם החדש כדי ליצור את השטח.
    addGameObject(std::make_unique<Terrain>(m_world, worldSize));

    // --- סוף התיקון ---

    // כעת, נמקם את השחקן הראשון איפשהו בתחילת העולם הגדול.
    m_players.push_back(std::make_unique<Player>(m_world, *this, sf::Vector2f(200.f, 500.f), sf::Color::Red));
    m_players.push_back(std::make_unique<Player>(m_world, *this, sf::Vector2f(800.f, 500.f), sf::Color::Green));
    std::cout << "Players number " << m_players.size() << std::endl;

    // --- HUD init (משתמש בפונט info_font מה-ResourceGraphic) --- // NEW
    try {
        auto& font = ResourceGraphic::getInstance().getFont("main_font");
        m_hudText.setFont(font);
        m_hudText.setCharacterSize(22);
        m_hudText.setFillColor(sf::Color::White);
        m_hudText.setOutlineColor(sf::Color::Black);
        m_hudText.setOutlineThickness(2.f);
        m_hudReady = true;
    }
    catch (const std::exception& e) {
        std::cerr << "HUD font load failed: " << e.what() << "\n";
        m_hudReady = false;
    }

    // --- התחלת תור ראשון (טיימר) --- // NEW
    m_currentPlayerIndex = 0;
    m_turnTimer = m_turnTimeDefault;
    startTurn();
}

void GameController::update(sf::Time deltaTime) {
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    m_world.Step(deltaTime.asSeconds(), velocityIterations, positionIterations);

    // אם יש בקשה לסיים תור (למשל אחרי ירי/נשק) – נסיים מיד
    if (m_endTurnRequested) {
        m_endTurnRequested = false;
        endTurn();
        return; // לא ממשיכים לעדכן בפריים של חילוף תור
    }

    // --- הורדת טיימר תור ומעבר כשנגמר --- // NEW
    if (!m_players.empty()) {
        m_turnTimer -= deltaTime.asSeconds();
        if (m_turnTimer <= 0.f) {
            endTurn();
            return; // לא ממשיכים לעדכן בפריים של חילוף תור
        }
    }

    // עדכון כל אובייקטי המשחק
    for (auto& object : m_gameObjects) {
        object->update(deltaTime);
    }

    if (!m_players.empty()) {
        Player* activePlayer = m_players[m_currentPlayerIndex].get();
        sf::Vector2f playerPosition = activePlayer->getPosition();
        m_cameraView.setCenter(playerPosition);
    }

    // ===================================================================
    // =========== כאן נמצא הקוד החדש שהוספנו ============
    // ===================================================================
    // הקוד הזה מניח שהוספת את 'm_terrain_ptr' לקובץ ה-header
    // ואתחלת אותו בבנאי, כפי שהסברתי קודם.

    sf::Vector2f center = m_cameraView.getCenter();
    sf::Vector2f halfSize = m_cameraView.getSize() / 2.f;

    // קבלת מידות המפה בפיקסלים מהמצביע ששמרנו
    sf::Vector2u windowSize = m_window.getSize();
    sf::Vector2f mapSizePixels(windowSize.x * 4, windowSize.y);

    // הגבלת המצלמה לגבולות
    if (center.x - halfSize.x < 0) { center.x = halfSize.x; }
    if (center.x + halfSize.x > mapSizePixels.x) { center.x = mapSizePixels.x - halfSize.x; }
    if (center.y - halfSize.y < 0) { center.y = halfSize.y; }
    if (center.y + halfSize.y > mapSizePixels.y) { center.y = mapSizePixels.y - halfSize.y; }

    // עדכון סופי של מרכז המצלמה לאחר ההגבלה
    m_cameraView.setCenter(center);

    // ===================================================================
    // ================= סוף הקוד החדש =================
    // ===================================================================

    m_cameraView.setSize(m_window.getSize().x, m_window.getSize().y);

    // כאן אנו מסירים את כל האובייקטים שסומנו כ"מתים"
    m_gameObjects.erase(
        std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
            [this](const std::unique_ptr<GameObject>& obj) {
                if (obj->isDead()) {
                    if (obj->getBody()) {
                        m_world.DestroyBody(obj->getBody());
                    }
                    return true;
                }
                return false;
            }),
        m_gameObjects.end());
}

void GameController::run() {
    sf::Clock clock;
    // אם מציק דילוגים כפולים: m_window.setKeyRepeatEnabled(false);

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }

            // Enter מסיים תור ידנית // NEW (אופציונלי)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                endTurn();
                continue; // לא להעביר ל-Player
            }

            if (!m_players.empty()) {
                m_players[m_currentPlayerIndex]->handleInput(event);
            }
        }

        if (!m_players.empty()) {
            m_players[m_currentPlayerIndex]->handleInput(sf::Event{});
        }

        sf::Time deltaTime = clock.restart();
        update(deltaTime);
        render();
    }
}

void GameController::handleEvents() {
    // This is now handled inside run()
}

void GameController::render() {
    // 1. נקה את המסך לצבע רקע בסיסי.
    m_window.clear(sf::Color(135, 206, 235));

    // 2. הגדר את המצלמה (View) לעקוב אחרי הדמות.
    m_window.setView(m_cameraView);

    // 3. הרקע
    sf::Vector2f viewCenter = m_cameraView.getCenter();
    sf::Vector2f viewSize = m_cameraView.getSize();
    m_skySprite.setPosition(viewCenter.x - viewSize.x / 2.f, viewCenter.y - viewSize.y / 2.f);
    m_window.draw(m_skySprite);

    // 4. אובייקטי המשחק
    for (const auto& object : m_gameObjects) {
        object->render(m_window);
    }

    // 5. HUD למעלה באמצע // NEW
    if (m_hudReady) {
        // נעבור ל-View ברירת מחדל כדי לצייר ביחידות מסך
        sf::View prev = m_window.getView();
        m_window.setView(m_window.getDefaultView());

        // טקסט: "Player X  |  Ys"
        int playerNum = m_players.empty() ? 0 : (m_currentPlayerIndex + 1);
        int secsLeft = static_cast<int>(std::ceil(std::max(0.f, m_turnTimer)));
        std::string hudStr = "Player " + std::to_string(playerNum) + "  |  " + std::to_string(secsLeft) + "s";
        m_hudText.setString(hudStr);

        // למקם באמצע-עליון
        sf::Vector2u win = m_window.getSize();
        sf::FloatRect tb = m_hudText.getLocalBounds();

        // רקע חצי-שקוף מאחורי הטקסט
        const float padX = 12.f, padY = 6.f;
        sf::RectangleShape bg;
        bg.setSize({ tb.width + 2.f * padX, tb.height + 2.f * padY });
        bg.setFillColor(sf::Color(0, 0, 0, 140));
        bg.setOutlineThickness(1.5f);
        bg.setOutlineColor(sf::Color(255, 255, 255, 80));

        // מיושרים לאמצע-עליון
        m_hudText.setOrigin(tb.left + tb.width / 2.f, tb.top);
        bg.setOrigin(bg.getSize().x / 2.f, 0.f);

        const float topMargin = 6.f;
        m_hudText.setPosition(static_cast<float>(win.x) / 2.f, topMargin + 4.f);
        bg.setPosition(static_cast<float>(win.x) / 2.f, topMargin);

        m_window.draw(bg);
        m_window.draw(m_hudText);

        // החזרת ה-View הקודם
        m_window.setView(prev);
    }

    // 6. הצג למסך
    m_window.display();
}

// ===================== Turn management ===================== // NEW

void GameController::startTurn() {
    if (m_players.empty()) return;
    m_turnTimer = m_turnTimeDefault; // איפוס הטיימר לתור
    std::cout << "[TURN] Player " << m_currentPlayerIndex << " started\n";
}

void GameController::endTurn() {
    if (m_players.empty()) return;
    m_currentPlayerIndex = (m_currentPlayerIndex + 1) % static_cast<int>(m_players.size());
    std::cout << "[TURN] Switched to Player " << m_currentPlayerIndex << "\n";
	startTurn();
}


void GameController::requestEndTurn() {
    m_endTurnRequested = true;
}