#pragma once

#include <vector>
#include <memory>
#include "Game/Object/Worm.h"

// Forward declaration כדי למנוע תלות מעגלית
class GameController;

class Player {
public:
    // הבנאי יקבל את העולם הפיזיקלי, הפניה לבקר המשחק, ומיקום התחלתי
    Player(b2World& world, GameController& gameController, const sf::Vector2f& basePosition);

    // טיפול בקלט עבור השחקן הנוכחי
    void handleInput(const sf::Event& event);

    // עדכון כל התולעים של השחקן (למרות שהלולאה הראשית כבר עושה זאת)
    void update(sf::Time deltaTime);

    // ציור כל התולעים של השחקן (וגם כאן, הלולאה הראשית תעשה זאת)
    void render(sf::RenderWindow& window);

private:
    void nextWorm();

    // וקטור של מצביעים לתולעים. הבעלות על התולעים עצמן תהיה של GameController
    std::vector<Worm*> m_worms;
    int m_currentWormIndex;

    GameController& m_gameController; // שמירת רפרנס לבקר
};