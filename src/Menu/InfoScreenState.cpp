#include "Menu/InfoScreenState.h"
#include "Menu/MenuManager.h"
#include "Menu/PopStateCommand.h"
#include <iostream>

InfoScreenState::InfoScreenState() : m_isPositionsSet(false) {}

// הפונקציה הזו הייתה חסרה! הוסף אותה.
void InfoScreenState::onEnter(MenuManager* manager) {
    // קריאה לפונקציה של מחלקת האב כדי לשמור את המצביע
    MenuScreenState::onEnter(manager);

    if (!m_font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font in InfoScreenState" << std::endl;
    }
}

void InfoScreenState::setupPositions(const sf::RenderWindow& window) {
    // עכשיו, כשהפונקציה הזו תיקרא, m_manager כבר יהיה תקין
    if (!m_manager) {
        std::cerr << "FATAL ERROR in InfoScreenState::setupPositions - m_manager is null" << std::endl;
        return;
    }

    sf::Vector2f windowSize(window.getSize());
    float centerX = windowSize.x / 2.f;

    // ... (שאר קוד המיקום נשאר אותו דבר)
    m_titleText.setFont(m_font);
    m_titleText.setString("Info & Credits");
    m_titleText.setCharacterSize(48);
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    m_titleText.setPosition(centerX, windowSize.y * 0.1f);

    std::wstring credits = L"Worms Game\n\n";
    credits += L"Created by:\n";
    credits += L"Alexey Laikov\n";
    credits += L"Talia Barzilai\n";
    credits += L"Iftah Ohayon\n\n";
    credits += L"OOP2 Project";

    m_infoText.setFont(m_font);
    m_infoText.setString(credits);
    m_infoText.setCharacterSize(24);
    sf::FloatRect textBounds = m_infoText.getLocalBounds();
    m_infoText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_infoText.setPosition(centerX, windowSize.y * 0.4f);

    sf::Vector2f buttonSize(200.f, 50.f);
    float buttonX = centerX - (buttonSize.x / 2.f);
    m_backButton.setup(L"Back", m_font, { buttonX, windowSize.y * 0.8f }, buttonSize);
    m_backButton.setCommand(std::make_unique<PopStateCommand>(*m_manager));
}

void InfoScreenState::render(sf::RenderWindow& window) {
    if (!m_isPositionsSet) {
        setupPositions(window);
        m_isPositionsSet = true;
    }

    window.draw(m_titleText);
    window.draw(m_infoText);
    m_backButton.render(window);
}

void InfoScreenState::handleInput(sf::Event& event, sf::RenderWindow& window) {
    m_backButton.handleEvent(event, window);
}

void InfoScreenState::onExit() {
    // לוגיקה שרצה ביציאה (אם צריך)
}