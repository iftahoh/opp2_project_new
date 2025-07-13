#include "Game/GameController.h"
#include "Game/Object/Terrain.h"
#include "Game/Player.h"
#include <iostream>

GameController::GameController(sf::RenderWindow& window)
    : m_window(window), m_world(b2Vec2(0.0f, 9.8f)), m_currentPlayerIndex(0)
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
    m_skySprite.setScale((float)windowSize.x / textureSize.x, (float)windowSize.y / textureSize.y);

    addGameObject(std::make_unique<Terrain>(m_world, m_window.getSize()));
    m_players.push_back(std::make_unique<Player>(m_world, *this, sf::Vector2f(200.f, 500.f)));
}

void GameController::update(sf::Time deltaTime) {
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    m_world.Step(deltaTime.asSeconds(), velocityIterations, positionIterations);

    // עדכון כל אובייקטי המשחק
    for (auto& object : m_gameObjects) {
        object->update(deltaTime);
    }

    // כאן אנו מסירים את כל האובייקטים שסומנו כ"מתים"
    m_gameObjects.erase(
        std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
            [this](const std::unique_ptr<GameObject>& obj) {
                if (obj->isDead()) {
                    // אם לאובייקט יש גוף פיזיקלי, אנו מסירים גם אותו מהעולם של Box2D.
                    if (obj->getBody()) {
                        m_world.DestroyBody(obj->getBody());
                    }
                    return true; // מסמן שיש להסיר את האובייקט מהווקטור.
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

    for (const auto& object : m_gameObjects) {
        object->render(m_window);
    }

    // Player rendering is handled by the GameObjects vector now
    // if (!m_players.empty()) {
    //     m_players[m_currentPlayerIndex]->render(m_window);
    // }

    m_window.display();
}
