#pragma once

#include "MenuScreenState.h"
#include <SFML/Graphics.hpp>
#include "Button.h" 

/*
MainMenuScreenState class representing the main menu screen state.
*/

class MainMenuScreenState : public MenuScreenState {
public:
	MainMenuScreenState();

	void render(sf::RenderWindow& window) override;
	void handleInput(sf::Event& event, sf::RenderWindow& window) override;
	void onEnter(MenuManager* manager) override;
	void onExit() override;

private:
	void setupPositions(sf::RenderWindow& window);

	sf::Font font;
	sf::Texture backgroundTexture;
	sf::Sprite background;

	Button startGameButton;
	Button settingsButton;
	Button infoButton;
	Button exitGameButton;

	bool m_isPositionsSet;
};