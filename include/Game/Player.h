#pragma once

#include <vector>
#include <memory>
#include "Game/Object/Worm.h"

// Forward declaration ��� ����� ���� ������
class GameController;

class Player {
public:
    // ����� ���� �� ����� ��������, ����� ���� �����, ������ ������
    Player(b2World& world, GameController& gameController, const sf::Vector2f& basePosition);

    // ����� ���� ���� ����� ������
    void handleInput(const sf::Event& event);

    // ����� �� ������� �� ����� (����� ������� ������ ��� ���� ���)
    void update(sf::Time deltaTime);

    // ���� �� ������� �� ����� (��� ���, ������ ������ ���� ���)
    void render(sf::RenderWindow& window);

private:
    void nextWorm();

    // ����� �� ������� �������. ������ �� ������� ���� ���� �� GameController
    std::vector<Worm*> m_worms;
    int m_currentWormIndex;

    GameController& m_gameController; // ����� ����� ����
};