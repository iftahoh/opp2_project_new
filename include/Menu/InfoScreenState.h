#pragma once

#include "MenuScreenState.h"
#include "Button.h"
#include "Move.h"

class InfoScreenState : public MenuScreenState {
public:
    InfoScreenState();

    void onEnter(MenuManager* manager) override;
    void render(sf::RenderWindow& window) override;
    void handleInput(sf::Event& event, sf::RenderWindow& window) override;
    void onExit() override;

private:
    void setupPositions(const sf::RenderWindow& window);

    sf::Font m_font;
    sf::Texture m_infoImage;
    Button m_backButton;
	Move m_nextInfoButton;
	Move m_previousInfoButton;
    bool m_isPositionsSet;
	int m_currentInfoIndex;
};