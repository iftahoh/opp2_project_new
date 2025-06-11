#pragma once

#include <SFML/Graphics.hpp>

class MenuManager; // Forward declaration

class MenuScreenState {
public:
    // הגדרת הבנאי ישירות בקובץ ההדר
    MenuScreenState() : m_manager(nullptr) {}
    virtual ~MenuScreenState() = default;

    // פונקציות וירטואליות טהורות - כל מחלקה שיורשת חייבת לממש אותן
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void handleInput(sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void onExit() = 0;

    // לפונקציה onEnter ניתן מימוש ברירת מחדל כדי לשמור את המצביע למנהל
    virtual void onEnter(MenuManager* manager) {
        m_manager = manager;
    }

protected:
    // מצביע למנהל התפריטים, זמין לכל המחלקות היורשות
    MenuManager* m_manager;
};