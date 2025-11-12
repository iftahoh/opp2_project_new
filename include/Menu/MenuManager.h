#pragma once

#include "MenuScreenState.h"
#include <stack>
#include <memory>
#include <type_traits>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp> // הוספנו את זה כאן

class MenuManager {
public:
    MenuManager() = default;
    ~MenuManager() = default;

    template <typename T>
    void pushState() {
        static_assert(std::is_base_of<MenuScreenState, T>::value, "T must be a derivative of MenuScreenState");

        std::unique_ptr<MenuScreenState> newState = std::make_unique<T>();
        if (!m_states.empty()) {
            // פעולה על המצב הישן
        }
        newState->onEnter(this);
        m_states.push(std::move(newState));
    }

    void popState(); // נשאיר רק הצהרה

    template <typename T>
    void changeState() {
        if (!m_states.empty()) {
            popState();
        }
        pushState<T>();
    }

    void handleInput(sf::Event& event, sf::RenderWindow& window); // נשאיר רק הצהרה
    void render(sf::RenderWindow& window); // נשאיר רק הצהרה

    bool isRunning() const;

private:
    std::stack<std::unique_ptr<MenuScreenState>> m_states;
};