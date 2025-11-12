#pragma once

#include "MenuScreenState.h"
#include <stack>
#include <memory>
#include <type_traits>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

/*
MenuManager class that manages the stack of menu screen states.
*/

class MenuManager {
public:
    MenuManager() = default;
    ~MenuManager() = default;

    template <typename T>
    void pushState() {
        static_assert(std::is_base_of<MenuScreenState, T>::value, "T must be a derivative of MenuScreenState");

        std::unique_ptr<MenuScreenState> newState = std::make_unique<T>();
        if (!m_states.empty()) {
        }
        newState->onEnter(this);
        m_states.push(std::move(newState));
    }

    void popState();

    template <typename T>
    void changeState() {
        if (!m_states.empty()) {
            popState();
        }
        pushState<T>();
    }

    void handleInput(sf::Event& event, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    bool isRunning() const;

private:
    std::stack<std::unique_ptr<MenuScreenState>> m_states;
};