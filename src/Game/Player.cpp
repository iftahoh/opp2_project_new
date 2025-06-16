#include "Game/Player.h"
#include "Game/GameController.h" // הוספת הקובץ המלא
#include <iostream>

const int NUM_WORMS_PER_PLAYER = 5;

Player::Player(b2World& world, GameController& gameController, const sf::Vector2f& basePosition)
    : m_currentWormIndex(0), m_gameController(gameController) {

    // יצירת התולעים והוספתן גם לווקטור המקומי (מצביעים) וגם לבקר הכללי
    for (int i = 0; i < NUM_WORMS_PER_PLAYER; ++i) {
        // מיקום התולעים אחת ליד השנייה
        sf::Vector2f wormPos = basePosition + sf::Vector2f(i * 50.f, 0.f);

        // ================== הוספת התיקון ==================
        // נזיז את התולעת מעט למטה מהמיקום המחושב,
        // בערך בגודל של רדיוס ה"קליפה" של Box2D.
        // b2_linearSlop הוא קבוע של Box2D המייצג מרחק קטן מאוד.
        // ניסיון טוב הוא להזיז אותה פי 2 או 3 מגודל זה.
        const float y_offset = 3.0f * b2_linearSlop * 30.0f; // 30.0f הוא ה-SCALE
        wormPos.y += y_offset;
        // ===============================================

        auto worm = std::make_unique<Worm>(world, wormPos);

        m_worms.push_back(worm.get());

        m_gameController.addGameObject(std::move(worm));
    }
}

void Player::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Tab) {
            nextWorm();
            return; // סיימנו לטפל באירוע זה
        }

        // ----- לוגיקה לבחירת נשק -----
        if (event.key.code == sf::Keyboard::Num1) {
            std::cout << "Equipping Bazooka (placeholder)" << std::endl;
            if (!m_worms.empty()) {
                // נצטרך ליצור מחלקת Bazooka אמיתית
                // auto bazooka = std::make_unique<Bazooka>();
                // m_worms[m_currentWormIndex]->equipWeapon(std::move(bazooka));
            }
        }
        // ... אפשר להוסיף כאן עוד מקשים לנשקים נוספים ...
    }

    // העברת קלט לשליטה בתולעת הפעילה
    if (!m_worms.empty()) {
        m_worms[m_currentWormIndex]->handlePlayerInput(event);
    }
}

void Player::nextWorm() {
    if (m_worms.empty()) return;

    // איפוס הכוח של התולעת הנוכחית כדי שלא תמשיך לזוז
    m_worms[m_currentWormIndex]->getBody()->SetLinearVelocity(b2Vec2(0, 0));

    // קידום האינדקס לתולעת הבאה
    m_currentWormIndex = (m_currentWormIndex + 1) % m_worms.size();
    std::cout << "Switched to worm " << m_currentWormIndex + 1 << std::endl;
}

// הפונקציות הבאות לא נחוצות אם הלולאה הראשית ב-GameController מעדכנת ומציירת
// את כל האובייקטים ב-m_gameObjects
void Player::update(sf::Time deltaTime) {
    // for (auto& worm : m_worms) {
    //     worm->update(deltaTime);
    // }
}

void Player::render(sf::RenderWindow& window) {
    // for (auto& worm : m_worms) {
    //     worm->render(window);
    // }
}