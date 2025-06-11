#pragma once

#include <SFML/Graphics.hpp>

class MenuManager; // Forward declaration

class MenuScreenState {
public:
    // ����� ����� ������ ����� ����
    MenuScreenState() : m_manager(nullptr) {}
    virtual ~MenuScreenState() = default;

    // �������� ���������� ������ - �� ����� ������ ����� ���� ����
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void handleInput(sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void onExit() = 0;

    // �������� onEnter ���� ����� ����� ���� ��� ����� �� ������ �����
    virtual void onEnter(MenuManager* manager) {
        m_manager = manager;
    }

protected:
    // ����� ����� ��������, ���� ��� ������� �������
    MenuManager* m_manager;
};