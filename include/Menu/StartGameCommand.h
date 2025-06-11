#pragma once

#include <Menu/ICommand.h>
#include <box2D/box2D.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game/GameController.h" // שונה ל-GameController.h

class StartGameCommand : public ICommand {
public:
    StartGameCommand(sf::RenderWindow& window) : m_window(window) {}
    ~StartGameCommand() override = default;

    void execute() override {
        std::cout << "Starting the game..." << std::endl;

        // יצירת אובייקט מהמחלקה החדשה
        GameController game(m_window);
        game.run();
    }

private:
    sf::RenderWindow& m_window;
};