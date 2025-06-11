#include "Game/GameController.h"
#include "Game/Object/Terrain.h"
#include "Game/Player.h"
#include <iostream>

GameController::GameController(sf::RenderWindow& window)
    : m_window(window), m_world(b2Vec2(0.0f, 9.8f)), m_currentPlayerIndex(0)
{
    setupWorld();
}

GameController::~GameController() = default;

void GameController::addGameObject(std::unique_ptr<GameObject> object) {
    m_gameObjects.push_back(std::move(object));
}

void GameController::setupWorld() {
    if (!m_skyTexture.loadFromFile("beach_background.png")) {
        std::cerr << "Error loading sky texture" << std::endl;
    }
    m_skySprite.setTexture(m_skyTexture);
    sf::Vector2u windowSize = m_window.getSize();
    sf::Vector2u textureSize = m_skyTexture.getSize();
    m_skySprite.setScale((float)windowSize.x / textureSize.x, (float)windowSize.y / textureSize.y);

    m_gameObjects.push_back(std::make_unique<Terrain>(m_world, m_window.getSize()));

    // שימוש במיקום התחלתי נכון (קרוב לאדמה)
    m_players.push_back(std::make_unique<Player>(m_world, *this, sf::Vector2f(200.f, 500.f)));
}

void GameController::update(sf::Time deltaTime) {
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    m_world.Step(deltaTime.asSeconds(), velocityIterations, positionIterations);

    for (auto& object : m_gameObjects) {
        object->update(deltaTime);
    }
}

void GameController::render() {
    m_window.clear(sf::Color(135, 206, 235));
    m_window.draw(m_skySprite);

    for (const auto& object : m_gameObjects) {
        object->render(m_window);
    }

    m_window.display();
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

void GameController::handleEvents(const sf::Event& event) {
    // הפונקציה הזו כרגע לא בשימוש בלולאה הראשית,
    // אבל נשאיר אותה למקרה הצורך
}