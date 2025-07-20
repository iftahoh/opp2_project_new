#pragma once

#include <vector>
#include <memory>
#include "Game/Object/Worm.h"

// Forward declaration ëãé ìîðåò úìåú îòâìéú
class GameController;

class Player {
public:
    // äáðàé é÷áì àú äòåìí äôéæé÷ìé, äôðéä ìá÷ø äîùç÷, åîé÷åí äúçìúé
    Player(b2World& world, GameController& gameController, const sf::Vector2f& basePosition);

    // èéôåì á÷ìè òáåø äùç÷ï äðåëçé
    void handleInput(const sf::Event& event);

    // òãëåï ëì äúåìòéí ùì äùç÷ï (ìîøåú ùäìåìàä äøàùéú ëáø òåùä æàú)
    void update(sf::Time deltaTime);

    // öéåø ëì äúåìòéí ùì äùç÷ï (åâí ëàï, äìåìàä äøàùéú úòùä æàú)
    void render(sf::RenderWindow& window);
	sf::Vector2f getPosition() const;

private:
    void nextWorm();

    // å÷èåø ùì îöáéòéí ìúåìòéí. äáòìåú òì äúåìòéí òöîï úäéä ùì GameController
    std::vector<Worm*> m_worms;
    int m_currentWormIndex;

    GameController& m_gameController; // ùîéøú øôøðñ ìá÷ø
};