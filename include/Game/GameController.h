#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include "Game/GameContactListener.h"
#include <memory>
#include <random>
#include <string>
#include "Game/Object/SceneryObject.h"

class GameObject;
class Player;

class GameController {
public:
    explicit GameController(sf::RenderWindow& window);
    ~GameController();
    void run();

    void addGameObject(std::unique_ptr<GameObject> object);
    b2World& getWorld();

	static constexpr float SCALE = 30.0f; // Box2D scale factor

    void requestEndTurn();  // קרא לזה כששחקן השתמש בנשק ורוצים לסיים תור
    void startTurn();   // איפוס טיימר/התחלת תור
    void endTurn();

    void setCameraFollowTarget(GameObject* target);
    void setTurnActionInProgress(bool status);

private:
    void handleEvents();
    void update(sf::Time deltaTime);
    void render();
    void setupWorld();

    GameContactListener m_contactListener;
    sf::RenderWindow& m_window;
    b2World m_world;

    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::vector<std::unique_ptr<Player>> m_players;
    int m_currentPlayerIndex;

    sf::Texture m_skyTexture;
    sf::Sprite m_skySprite;

	// --- camera ---
    sf::View m_cameraView;

    // --- Turn management (basic) ---
    float m_turnTimeDefault = 30.f;  // כמה שניות לתור
    float m_turnTimer = 30.f;  // הטיימר שרץ בפועל

    bool m_endTurnRequested = false; // דגל פנימי לבקשת סיום תור


    // -------- HUD -------- // NEW
    sf::Text m_hudText;          // טקסט ל-HUD
    bool     m_hudReady = false; // נדע אם הטעינה הצליחה
};