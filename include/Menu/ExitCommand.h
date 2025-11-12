#pragma once

#include "ICommand.h"
#include <SFML/Graphics.hpp>

/*
* ExitCommand class that implements the ICommand interface to close the application window.
*/

class ExitCommand : public ICommand {
public:
    ExitCommand(sf::RenderWindow& window) : m_window(window) {}

    void execute() override {
        m_window.close();
    }

private:
    sf::RenderWindow& m_window;
};