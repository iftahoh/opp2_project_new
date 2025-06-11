#include "Menu/MenuManager.h"

void MenuManager::popState() {
    if (!m_states.empty()) {
        m_states.top()->onExit();
        m_states.pop();
    }
}

void MenuManager::handleInput(sf::Event& event, sf::RenderWindow& window) {
    if (!m_states.empty()) {
        m_states.top()->handleInput(event, window);
    }
}

void MenuManager::render(sf::RenderWindow& window) {
    if (!m_states.empty()) {
        m_states.top()->render(window);
    }
}

bool MenuManager::isRunning() const {
    return !m_states.empty();
}
