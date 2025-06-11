#include <SFML/Graphics.hpp>
#include "Menu/MenuManager.h"
#include "Menu/MainMenuScreenState.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "OOP2 Project");
    window.setFramerateLimit(60);

    MenuManager menuManager;
    menuManager.pushState<MainMenuScreenState>();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            menuManager.handleInput(event, window);
        }

        window.clear(sf::Color::Black);
        menuManager.render(window);
        window.display();
    }

    return 0;
}