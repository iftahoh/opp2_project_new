#include "Game/Player.h"
#include "Game/GameController.h"
#include "Game/Object/Weapon/Bazooka.h"
#include "Game/Object/Weapon/Grenade.h"
#include <iostream>

std::uniform_real_distribution<> distrX(-1900.f, 1900.f);
std::random_device rd;
std::mt19937 gen(rd());

const int NUM_WORMS_PER_PLAYER = 5;

Player::Player(b2World& world, GameController& gameController, const sf::Vector2f& basePosition, sf::Color color)
	: m_currentWormIndex(0), m_gameController(gameController), m_color(color), m_selectedWeaponIndex(0) {

    for (int i = 0; i < NUM_WORMS_PER_PLAYER; ++i) {
        float randomOffsetX = distrX(gen);
        sf::Vector2f wormPos = basePosition + sf::Vector2f(randomOffsetX, 0.f);

        const float y_offset = 3.0f * b2_linearSlop * 30.0f;
        wormPos.y += y_offset;

        auto worm = std::make_unique<Worm>(world, m_gameController, wormPos, m_color);

        m_worms.push_back(worm.get());
        m_gameController.addGameObject(std::move(worm));
    }

    m_weaponInventory.push_back("Bazooka");
    m_weaponInventory.push_back("Grenade");
}

Player::Player(b2World& world, GameController& gameController, const sf::Vector2f& basePosition, sf::Color color, int wormNum)
    : m_currentWormIndex(0), m_gameController(gameController), m_color(color), m_selectedWeaponIndex(0) {
    for (int i = 0; i < wormNum; ++i) {
        float randomOffsetX = distrX(gen);
        sf::Vector2f wormPos = basePosition + sf::Vector2f(randomOffsetX, 0.f);

        const float y_offset = 3.0f * b2_linearSlop * 30.0f;
        wormPos.y += y_offset;

        auto worm = std::make_unique<Worm>(world, m_gameController, wormPos, m_color);

        m_worms.push_back(worm.get());
        m_gameController.addGameObject(std::move(worm));
    }

    m_weaponInventory.push_back("Bazooka");
    m_weaponInventory.push_back("Grenade");
}

void Player::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        // --- עדכון לוגיקה ---

        // החלפת נשק (למשל מקש 1 ו-2)
        if (event.key.code == sf::Keyboard::Num1) {
            if (m_weaponInventory.size() > 0) m_selectedWeaponIndex = 0;
            return; // סיימנו טיפול בקלט
        }
        if (event.key.code == sf::Keyboard::Num2) {
            if (m_weaponInventory.size() > 1) m_selectedWeaponIndex = 1;
            return; // סיימנו טיפול בקלט
        }
        // (אפשר להוסיף 3, 4 וכו' בהמשך)

        // מקש לציוד הנשק (למשל LShift)
        if (event.key.code == sf::Keyboard::LShift) {
            if (m_worms.empty() || m_weaponInventory.empty()) return;

            std::string selectedWeaponName = m_weaponInventory[m_selectedWeaponIndex];

            // ניצור את הנשק הנבחר
            std::unique_ptr<IWeapon> weaponToEquip;
            if (selectedWeaponName == "Bazooka") {
                weaponToEquip = std::make_unique<Bazooka>();
            }
            else if (selectedWeaponName == "Grenade") {
                weaponToEquip = std::make_unique<Grenade>();
            }
            // (נוסיף עוד 'else if' לנשקים עתידיים)

            // נצייד את התולעת הפעילה
            if (weaponToEquip) {
                m_worms[m_currentWormIndex]->equipWeapon(std::move(weaponToEquip));
            }
            return;
        }
        // --- סוף עדכון ---
    }

    if (!m_worms.empty()) {
        m_worms[m_currentWormIndex]->handlePlayerInput(event);
    }
}

void Player::nextWorm() {
    if (m_worms.empty()) return;

    // לעצור את התולעת הנוכחית
    if (m_worms[m_currentWormIndex] && m_worms[m_currentWormIndex]->getBody()) {
        m_worms[m_currentWormIndex]->getBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
    }

    const std::size_t n = m_worms.size();
    std::size_t attempts = 0;

    // נתקדם קדימה עד שנמצא תולעת חיה או שנעשה סיבוב מלא
    do {
        m_currentWormIndex = (m_currentWormIndex + 1) % n;
        ++attempts;
    } while (attempts < n && m_worms[m_currentWormIndex] && m_worms[m_currentWormIndex]->isGrave());

    // אם כולן קברים
    if (m_worms[m_currentWormIndex]->isGrave()) {
        std::cout << "No alive worms for this player.\n";
        return;
    }

    std::cout << "Switched to worm " << (m_currentWormIndex + 1) << std::endl;
}


void Player::update(sf::Time deltaTime) {
}

void Player::render(sf::RenderWindow& window) {
}

sf::Vector2f Player::getPosition() const {
	b2Vec2 pos = m_worms[m_currentWormIndex]->getBody()->GetPosition();
	return sf::Vector2f(pos.x * GameController::SCALE, pos.y * GameController::SCALE);
}

int Player::getTotalHealth() const
{
    int totalHealth = 0;
    for (const Worm* worm : m_worms) {
        // נוודא שהתולעת לא במצב "קבר"
        if (!worm->isGrave()) {
            // נשתמש ב-getter שהוספנו ל-Worm
            totalHealth += worm->getHealth();
        }
    }
    // נחזיר 0 אם הסכום ירד מתחת לאפס
    return std::max(0, totalHealth);
}

const std::vector<std::string>& Player::getWeaponInventory() const {
    return m_weaponInventory;
}

int Player::getSelectedWeaponIndex() const {
    return m_selectedWeaponIndex;
}