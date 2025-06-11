#include "Menu/InfoScreenState.h"
#include "Menu/MenuManager.h"
#include "Menu/PopStateCommand.h"
#include "ResourceGraphic.h"
#include <iostream>

InfoScreenState::InfoScreenState() : m_isPositionsSet(false) {}

// הפונקציה הזו הייתה חסרה! הוסף אותה.
void InfoScreenState::onEnter(MenuManager* manager) {
    // קריאה לפונקציה של מחלקת האב כדי לשמור את המצביע
    MenuScreenState::onEnter(manager);

    if (!m_font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font in InfoScreenState" << std::endl;
    }
    m_infoImage = ResourceGraphic::getInstance().getTexture("background_info");
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
    sf::Sprite bg;
    bg.setTexture(m_infoImage);
    sf::Vector2u textureSize = m_infoImage.getSize();

    // חישוב קנה מידה - תמלא את כל החלון (גם אם זה מעוות)
    float scaleX = static_cast<float>(WINDOW_WIDTH) / textureSize.x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / textureSize.y;

    bg.setScale(scaleX, scaleY);
    window.draw(bg);
    m_backButton.render(window);
}

void InfoScreenState::handleInput(sf::Event& event, sf::RenderWindow& window) {
    m_backButton.handleEvent(event, window);
}

void InfoScreenState::onExit() {
    // לוגיקה שרצה ביציאה (אם צריך)
}