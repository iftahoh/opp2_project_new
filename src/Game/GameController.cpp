// src/Game/GameController.cpp
#include "Game/GameController.h"
#include "Game/Object/Terrain.h" // הוספת המחלקות החדשות
#include "Game/Object/Ball.h"
#include <iostream>

GameController::GameController(sf::RenderWindow& window)
    : m_window(window), m_world(b2Vec2(0.0f, 9.8f))
{
    setupWorld();
}

GameController::~GameController() = default;

void GameController::setupWorld() {
    // טעינת הרקע
    if (!m_skyTexture.loadFromFile("beach_background.png")) {
        std::cerr << "Error loading sky texture" << std::endl;
    }
    m_skySprite.setTexture(m_skyTexture);
    sf::Vector2u windowSize = m_window.getSize();
    sf::Vector2u textureSize = m_skyTexture.getSize();
    m_skySprite.setScale((float)windowSize.x / textureSize.x, (float)windowSize.y / textureSize.y);

    // יצירת האובייקטים והוספתם לרשימה
    m_gameObjects.push_back(std::make_unique<Terrain>(m_world, m_window.getSize()));
    m_gameObjects.push_back(std::make_unique<Ball>(m_world, sf::Vector2f(m_window.getSize().x / 2.f, 100.f)));
}

void GameController::update(sf::Time deltaTime) {
    // עדכון עולם הפיזיקה
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    m_world.Step(deltaTime.asSeconds(), velocityIterations, positionIterations);

    // עדכון כל האובייקטים במשחק
    for (auto& object : m_gameObjects) {
        object->update(deltaTime);
    }
}

void GameController::render() {
    m_window.clear(sf::Color(135, 206, 235)); // Sky blue

    // ציור הרקע
    m_window.draw(m_skySprite);

    // ציור כל האובייקטים במשחק
    for (const auto& object : m_gameObjects) {
        object->render(m_window);
    }

    m_window.display();
}

void GameController::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        handleEvents();

        sf::Time deltaTime = clock.restart();
        update(deltaTime);

        render();
    }
}

void GameController::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
    }
}