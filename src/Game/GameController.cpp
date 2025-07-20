#include "Game/GameController.h"
#include "Game/Object/Terrain.h"
#include "Game/Player.h"
#include <iostream>

GameController::GameController(sf::RenderWindow& window)
	: m_window(window),
    m_world(b2Vec2(0.0f, 9.8f)),
    m_cameraView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)),
    m_currentPlayerIndex(0)
{
    m_world.SetContactListener(&m_contactListener);
    setupWorld();
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
    m_skySprite.setScale((float)(windowSize.x / textureSize.x) *4, (float)windowSize.y / textureSize.y);

    // --- התיקון ---
    // 1. הגדר את גודל עולם המשחק הרצוי. בוא ניצור עולם שרוחבו פי 4 מהחלון.
    sf::Vector2u worldSize(windowSize.x * 4, windowSize.y);

    // 2. השתמש בגודל העולם החדש כדי ליצור את השטח.
    addGameObject(std::make_unique<Terrain>(m_world, worldSize));

    // --- סוף התיקון ---

    // כעת, נמקם את השחקן הראשון איפשהו בתחילת העולם הגדול.
    m_players.push_back(std::make_unique<Player>(m_world, *this, sf::Vector2f(200.f, 500.f)));
}

void GameController::update(sf::Time deltaTime) {
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    m_world.Step(deltaTime.asSeconds(), velocityIterations, positionIterations);

    // òãëåï ëì àåáéé÷èé äîùç÷
    for (auto& object : m_gameObjects) {
        object->update(deltaTime);
    }

    if (!m_players.empty()) {
        Player* activePlayer = m_players[m_currentPlayerIndex].get();
        sf::Vector2f playerPosition = activePlayer->getPosition(); // שימוש בפונקציה החדשה
        m_cameraView.setCenter(playerPosition);
    }
    m_cameraView.setSize(m_window.getSize().x, m_window.getSize().y);

    // ëàï àðå îñéøéí àú ëì äàåáéé÷èéí ùñåîðå ë"îúéí"
    m_gameObjects.erase(
        std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
            [this](const std::unique_ptr<GameObject>& obj) {
                if (obj->isDead()) {
                    // àí ìàåáéé÷è éù âåó ôéæé÷ìé, àðå îñéøéí âí àåúå îäòåìí ùì Box2D.
                    if (obj->getBody()) {
                        m_world.DestroyBody(obj->getBody());
                    }
                    return true; // îñîï ùéù ìäñéø àú äàåáéé÷è îäåå÷èåø.
                }
                return false;
            }),
        m_gameObjects.end());
}

void GameController::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
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
    m_window.clear(sf::Color(135, 206, 235));
    m_window.draw(m_skySprite);

    m_window.setView(m_cameraView);

    for (const auto& object : m_gameObjects) {
        object->render(m_window);
    }

    // Player rendering is handled by the GameObjects vector now
    // if (!m_players.empty()) {
    //     m_players[m_currentPlayerIndex]->render(m_window);
    // }

    m_window.display();
}
