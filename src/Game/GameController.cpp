#include "Game/GameController.h"
#include "Game/Object/Terrain.h"
#include "Game/Player.h"
#include "ResourceGraphic.h"                 // NEW: כדי להשתמש בפונט הטעון
#include <iostream>
#include <Menu/SettingsManager.h>

std::uniform_int_distribution<> distrX(2000.f, 3200.f);
// בקובץ GameController.cpp
GameController::GameController(sf::RenderWindow& window)
    : m_window(window),
    m_world(b2Vec2(0.0f, 9.8f)), // m_world הוא אכן ה-b2World, מצוין!
    m_cameraView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)),
    m_currentPlayerIndex(0), m_cameraTarget(nullptr), m_isGameOver(false)
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
    if (!m_skyTexture.loadFromFile("beach_background.png")) { //
        std::cerr << "Error loading sky texture" << std::endl; //
    }
    m_skySprite.setTexture(m_skyTexture); //
    sf::Vector2u windowSize = m_window.getSize(); //
    sf::Vector2u textureSize = m_skyTexture.getSize(); //
    m_skySprite.setScale((float)(windowSize.x / textureSize.x), (float)windowSize.y / textureSize.y); //

    // --- התיקון ---
    // 1. הגדר את גודל עולם המשחק הרצוי. בוא ניצור עולם שרוחבו פי 4 מהחלון.
    sf::Vector2u worldSize(windowSize.x * 4, windowSize.y); //

    // 2. ניצור את השטח ונשמור מצביע אליו
    // (חשוב: הוספנו משתנה מקומי m_terrain כדי שנוכל לגשת אליו)
    auto terrainPtr = std::make_unique<Terrain>(m_world, worldSize);
    Terrain* m_terrain = terrainPtr.get(); // שמירת מצביע "גולמי"
    addGameObject(std::move(terrainPtr)); // העברת הבעלות ל-m_gameObjects

    // =======================================================
    // ========= קוד פיזור אובייקטים (עם תיקון Y) =========
    // =======================================================

    std::random_device rd;
    std::mt19937 gen(rd());

    // הגדרת טווח X
    std::uniform_real_distribution<> distrX(100.f, static_cast<float>(worldSize.x) - 100.f);

    // (מחקנו את distrY)

    // רשימת הטקסטורות
    std::vector<std::string> sceneryKeys = { "texture1", "texture2", "texture3", "texture4"};

    // מספר האובייקטים
    const int numObjectsToSpawn = 15;

    std::uniform_int_distribution<> distrIndex(0, sceneryKeys.size() - 1);

    // רשימה לבדיקת חפיפות
    std::vector<sf::FloatRect> spawnedObjectBounds;
    const float PADDING = 20.0f; // מרווח ביטחון

    for (int i = 0; i < numObjectsToSpawn; ++i) {

        int retries = 0;
        const int MAX_RETRIES = 20; // ננסה למצוא מקום פנוי עד 20 פעם
        bool positionFound = false;

        while (retries < MAX_RETRIES && !positionFound) {
            retries++;

            // 1. נבחר טקסטורה אקראית ונקבל את הגודל שלה
            std::string randomTextureKey = sceneryKeys[distrIndex(gen)];
            const sf::Texture& texture = ResourceGraphic::getInstance().getTexture(randomTextureKey);
            sf::Vector2u textureSize = texture.getSize();
            float objHeight = static_cast<float>(textureSize.y);
            float objWidth = static_cast<float>(textureSize.x);

            // 2. נבחר X אקראי
            float randomX = distrX(gen);

            // 3. נשאל את ה-Terrain מה גובה הקרקע ב-X הזה
            float groundY = m_terrain->getSurfaceY(randomX);

            // 4. נחשב את ה-Y של מרכז האובייקט
            // (גובה הקרקע פחות חצי גובה האובייקט)
            float randomY = groundY - (objHeight / 2.f);

            sf::Vector2f randomPos(randomX, randomY);

            // 5. ניצור את ה"תיבה החוסמת" (bounding box)
            sf::FloatRect newBounds(
                randomPos.x - (objWidth / 2.f) - PADDING,
                randomPos.y - (objHeight / 2.f) - PADDING,
                objWidth + (PADDING * 2),
                objHeight + (PADDING * 2)
            );

            // 6. נבדוק אם התיבה החדשה מתנגשת עם תיבות קיימות
            bool isOverlapping = false;
            for (const auto& existingBounds : spawnedObjectBounds) {
                if (newBounds.intersects(existingBounds)) {
                    isOverlapping = true;
                    break;
                }
            }

            // 7. אם אין התנגשות - מצאנו מקום!
            if (!isOverlapping) {
                positionFound = true;

                // ניצור את האובייקט
                addGameObject(std::make_unique<SceneryObject>(m_world, randomPos, randomTextureKey));

                // ונוסיף את הגבולות שלו לרשימה
                spawnedObjectBounds.push_back(newBounds);
            }
        }
    }
    // =======================================================
    // =================== סוף הקוד החדש ===================
    // =======================================================
	int numOfWorms = SettingsManager::getInstance().getWormsPerPlayer(); //
    //מיקום שחקנים על המפה 
	float groundX = distrX(gen);
    m_players.push_back(std::make_unique<Player>(m_world, *this, sf::Vector2f(groundX, 300.f), sf::Color::Red, numOfWorms)); //
	groundX = distrX(gen);
    m_players.push_back(std::make_unique<Player>(m_world, *this, sf::Vector2f(groundX,300.f), sf::Color::Green, numOfWorms)); //
    std::cout << "Players number " << m_players.size() << std::endl; //

    // --- HUD init (משתמש בפונט info_font מה-ResourceGraphic) --- // NEW
    try {
        auto& font = ResourceGraphic::getInstance().getFont("main_font"); //
        
        // --- זה הקוד שהיה קיים, אבל עם השם החדש ---
        m_centerHudText.setFont(font);
        m_centerHudText.setCharacterSize(22);
        m_centerHudText.setFillColor(sf::Color::White);
        m_centerHudText.setOutlineColor(sf::Color::Black);
        m_centerHudText.setOutlineThickness(2.f);

        // --- הוספה: אתחול טקסטים חדשים ---
        m_player1HealthText = m_centerHudText; // העתקת הגדרות
        m_player2HealthText = m_centerHudText;
        m_weaponHudText = m_centerHudText;
        m_weaponHudText.setCharacterSize(18); // נשקים בקטן יותר
        // ----------------------------------

        m_hudReady = true; //

        m_winnerText.setFont(font);
        m_winnerText.setCharacterSize(60); // גופן גדול יותר
        m_winnerText.setFillColor(sf::Color::Yellow);
        m_winnerText.setOutlineColor(sf::Color::Black);
        m_winnerText.setOutlineThickness(3.f);
    }
    catch (const std::exception& e) {
        std::cerr << "HUD font load failed: " << e.what() << "\n"; //
        m_hudReady = false; //
    }

    // --- התחלת תור ראשון (טיימר) --- // NEW
    m_currentPlayerIndex = 0; //
    m_turnTimer = m_turnTimeDefault; //
    startTurn(); //
}

void GameController::checkWinCondition()
{
    if (m_isGameOver || m_players.size() < 2) {
        return;
    }

    // הנחה פשוטה שיש 2 שחקנים (באינדקס 0 ו-1)
    int player1Health = m_players[0]->getTotalHealth();
    int player2Health = m_players[1]->getTotalHealth();

    std::string winnerStr;

    if (player1Health <= 0) {
        winnerStr = "Player 2 Wins!";
        m_isGameOver = true;
    }
    else if (player2Health <= 0) {
        winnerStr = "Player 1 Wins!";
        m_isGameOver = true;
    }

    // אם המשחק נגמר בבדיקה הזו, נכין את טקסט הניצחון
    if (m_isGameOver) {
        std::cout << "GAME OVER: " << winnerStr << std::endl;
        m_winnerText.setString(winnerStr);
    }
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

    if (m_isGameOver) {
        return;
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
    checkWinCondition();

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
        sf::Vector2u win = m_window.getSize();
        const float topMargin = 6.f;

        // --- 5.א: טקסט בריאות (שמאל וימין) ---
        if (m_players.size() >= 2) {
            // שחקן 1 (שמאל)
            int p1Health = m_players[0]->getTotalHealth();
            m_player1HealthText.setString("P1 Health: " + std::to_string(p1Health));
            m_player1HealthText.setOrigin(0, 0); // יישור לשמאל-למעלה
            m_player1HealthText.setPosition(topMargin * 2.f, topMargin * 2.f);
            m_window.draw(m_player1HealthText);

            // שחקן 2 (ימין)
            int p2Health = m_players[1]->getTotalHealth();
            m_player2HealthText.setString("P2 Health: " + std::to_string(p2Health));
            sf::FloatRect p2b = m_player2HealthText.getLocalBounds();
            m_player2HealthText.setOrigin(p2b.left + p2b.width, 0); // יישור לימין-למעלה
            m_player2HealthText.setPosition(win.x - (topMargin * 2.f), topMargin * 2.f);
            m_window.draw(m_player2HealthText);
        }

        // --- 5.ב: טקסט מרכזי (טיימר ותור) ---
        int playerNum = m_players.empty() ? 0 : (m_currentPlayerIndex + 1);
        int secsLeft = static_cast<int>(std::ceil(std::max(0.f, m_turnTimer)));
        std::string hudStr = "Player " + std::to_string(playerNum) + "  |  " + std::to_string(secsLeft) + "s";
        m_centerHudText.setString(hudStr); // <-- שינוי שם

        sf::FloatRect tb = m_centerHudText.getLocalBounds(); // <-- שינוי שם

        // רקע חצי-שקוף מאחורי הטקסט
        const float padX = 12.f, padY = 6.f;
        sf::RectangleShape bg;
        bg.setSize({ tb.width + 2.f * padX, tb.height + 2.f * padY });
        bg.setFillColor(sf::Color(0, 0, 0, 140));
        bg.setOutlineThickness(1.5f);
        bg.setOutlineColor(sf::Color(255, 255, 255, 80));

        m_centerHudText.setOrigin(tb.left + tb.width / 2.f, tb.top); // <-- שינוי שם
        bg.setOrigin(bg.getSize().x / 2.f, 0.f);

        m_centerHudText.setPosition(static_cast<float>(win.x) / 2.f, topMargin + 4.f); // <-- שינוי שם
        bg.setPosition(static_cast<float>(win.x) / 2.f, topMargin);

        m_window.draw(bg);
        m_window.draw(m_centerHudText); // <-- שינוי שם

        // --- 5.ג: טקסט נשקים (מתחת למרכזי) ---
        if (!m_players.empty()) {
            Player* activePlayer = m_players[m_currentPlayerIndex].get();
            const auto& inventory = activePlayer->getWeaponInventory();
            int selectedIndex = activePlayer->getSelectedWeaponIndex();

            std::string weaponStr;
            for (int i = 0; i < inventory.size(); ++i) {
                if (i == selectedIndex) {
                    weaponStr += " > " + inventory[i] + " < ";
                }
                else {
                    weaponStr += "   " + inventory[i] + "   ";
                }
            }
            m_weaponHudText.setString(weaponStr);

            // מיקום מתחת לרקע של הטיימר
            sf::FloatRect wpnBounds = m_weaponHudText.getLocalBounds();
            m_weaponHudText.setOrigin(wpnBounds.left + wpnBounds.width / 2.f, wpnBounds.top);
            m_weaponHudText.setPosition(static_cast<float>(win.x) / 2.f, topMargin + bg.getSize().y + 5.f);

            m_window.draw(m_weaponHudText);
        }

        // החזרת ה-View הקודם
        m_window.setView(prev);
    }

    if (m_isGameOver && m_hudReady) { // m_hudReady מוודא שהפונט נטען
        // נחזור ל-View הרגיל כדי לצייר על כל המסך
        sf::View prev = m_window.getView();
        m_window.setView(m_window.getDefaultView());

        // רקע שחור חצי-שקוף
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(m_window.getSize()));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        m_window.draw(overlay);

        // טקסט ניצחון ממורכז
        sf::FloatRect tb = m_winnerText.getLocalBounds();
        m_winnerText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
        m_winnerText.setPosition(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f);
        m_window.draw(m_winnerText);

        // נשחזר את ה-View של המשחק
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