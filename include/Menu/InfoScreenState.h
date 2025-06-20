#pragma once

#include "MenuScreenState.h"
#include "Button.h"

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
    bool m_isPositionsSet;
};