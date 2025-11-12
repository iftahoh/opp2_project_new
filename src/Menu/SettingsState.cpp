#include "Menu/SettingsState.h"
#include "Menu/PopStateCommand.h"
#include "ResourceGraphic.h"
#include "Menu/SettingsManager.h"
#include "Menu/IncreaseWormsCommand.h"
#include "Menu/DecreseWormsCommand.h"
#include <iostream>

SettingsState::SettingsState() : m_isPositionsSet(false) {}

void SettingsState::onEnter(MenuManager* manager) {
    MenuScreenState::onEnter(manager);
    m_textFont = ResourceGraphic::getInstance().getFont("info_font");
	m_numberFont = ResourceGraphic::getInstance().getFont("main_font");
    m_settingsImage = ResourceGraphic::getInstance().getTexture("settings_screen");
}

void SettingsState::setupPositions(const sf::RenderWindow& window) {
    if (!m_manager) {
        std::cerr << "FATAL ERROR in SettingsState::setupPositions - m_manager is null" << std::endl;
        return;
    }

    sf::Vector2f windowSize(window.getSize());
    float centerX = windowSize.x / 2.f;
    float currentY = 250.f;

    const float labelX = 300.f;
    const float valueX = centerX + 100.f;
    const sf::Vector2f smallButtonSize(40.f, 40.f);

    m_wormsLabel.setFont(m_textFont);
    m_wormsLabel.setString("Worms Per Player");
    m_wormsLabel.setCharacterSize(35);
    m_wormsLabel.setPosition(labelX, currentY);

    m_wormsValueText.setFont(m_numberFont);
    m_wormsValueText.setCharacterSize(40);
    m_wormsValueText.setPosition(valueX + 35.f, currentY + 3.f);

    m_wormsDownButton.setup(L"-", m_textFont, { valueX - 50.f, currentY + 10.f }, smallButtonSize);
	m_wormsDownButton.setCommand(std::make_unique<class DecreseWormsCommand>());
    m_wormsUpButton.setup(L"+", m_textFont, { valueX + 100.f, currentY + 10.f }, smallButtonSize);
	m_wormsUpButton.setCommand(std::make_unique<class IncreaseWormsCommand>());

    sf::Vector2f buttonSize(150.f, 50.f);
    float buttonX = centerX - (buttonSize.x / 2.f);
    m_backButton.setup(L"Menu", m_textFont, { buttonX, (windowSize.y * 0.8f) + 85.f }, buttonSize);
    m_backButton.setCommand(std::make_unique<PopStateCommand>(*m_manager));

    updateTextValues();
}

void SettingsState::updateTextValues() {
    int currentWorms = SettingsManager::getInstance().getWormsPerPlayer();
    m_wormsValueText.setString(std::to_string(currentWorms));
}

void SettingsState::render(sf::RenderWindow& window) {
    if (!m_isPositionsSet) {
        setupPositions(window);
        m_isPositionsSet = true;
    }
    sf::Sprite bg;
    bg.setTexture(m_settingsImage);
    sf::Vector2u textureSize = m_settingsImage.getSize();

    float scaleX = static_cast<float>(WINDOW_WIDTH) / textureSize.x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / textureSize.y;

	updateTextValues();
    bg.setScale(scaleX, scaleY);
    window.draw(bg);
	window.draw(m_wormsLabel);
	window.draw(m_wormsValueText);
    m_backButton.render(window);
    m_wormsUpButton.render(window);
    m_wormsDownButton.render(window);
}

void SettingsState::handleInput(sf::Event& event, sf::RenderWindow& window) {
    m_wormsUpButton.handleEvent(event, window);
    m_wormsDownButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);
}

void SettingsState::onExit() {
    
}