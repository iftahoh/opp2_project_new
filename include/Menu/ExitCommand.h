#pragma once

#include "ICommand.h"
#include <SFML/Graphics.hpp>

// פקודה זו מקבלת מצביע לחלון כדי שתוכל לסגור אותו
class ExitCommand : public ICommand {
public:
    // הבנאי מקבל את החלון שעליו הוא צריך לפעול
    ExitCommand(sf::RenderWindow& window) : m_window(window) {}

    // הפעולה שמתבצעת היא פשוט לסגור את החלון
    void execute() override {
        m_window.close();
    }

private:
    sf::RenderWindow& m_window;
};