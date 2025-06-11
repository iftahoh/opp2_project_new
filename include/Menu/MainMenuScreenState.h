#pragma once

#include "MenuScreenState.h"
#include <SFML/Graphics.hpp>
#include "Button.h" 

class MainMenuScreenState : public MenuScreenState {
public:
	MainMenuScreenState();

	void render(sf::RenderWindow& window) override;
	void handleInput(sf::Event& event, sf::RenderWindow& window) override;
	void onEnter(MenuManager* manager) override;
	void onExit() override;

private:
	// פונקציית עזר פרטית למיקום האלמנטים
	// === התיקון כאן: הסרת המילה const ===
	void setupPositions(sf::RenderWindow& window);

	sf::Font font;
	sf::Text titleText;
	sf::Texture backgroundTexture;
	sf::Sprite background;

	Button startGameButton;
	Button settingsButton;
	Button infoButton;
	Button exitGameButton;

	bool m_isPositionsSet;
};