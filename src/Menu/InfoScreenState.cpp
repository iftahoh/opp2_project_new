#include "Menu/InfoScreenState.h"
#include "Menu/MenuManager.h"
#include "Menu/PopStateCommand.h"
#include "ResourceGraphic.h"
#include <iostream>

InfoScreenState::InfoScreenState() : m_isPositionsSet(false) {
    m_currentInfoIndex = 1;
}

// äôåð÷öéä äæå äééúä çñøä! äåñó àåúä.
void InfoScreenState::onEnter(MenuManager* manager) {
    // ÷øéàä ìôåð÷öéä ùì îçì÷ú äàá ëãé ìùîåø àú äîöáéò
    MenuScreenState::onEnter(manager);
	m_font = ResourceGraphic::getInstance().getFont("info_font");
    m_infoImage = ResourceGraphic::getInstance().getTexture("settings_screen");
}

void InfoScreenState::setupPositions(const sf::RenderWindow& window) {
    // òëùéå, ëùäôåð÷öéä äæå úé÷øà, m_manager ëáø éäéä ú÷éï
    if (!m_manager) {
        std::cerr << "FATAL ERROR in InfoScreenState::setupPositions - m_manager is null" << std::endl;
        return;
    }

    sf::Vector2f windowSize(window.getSize());
    float centerX = windowSize.x / 2.f;

    // ... (ùàø ÷åã äîé÷åí ðùàø àåúå ãáø)

  

    sf::Vector2f buttonSize(150.f, 50.f);
    float buttonX = centerX - (buttonSize.x / 2.f);
    m_backButton.setup(L"Menu", m_font, { buttonX, (windowSize.y * 0.8f) + 85.f }, buttonSize);
    m_backButton.setCommand(std::make_unique<PopStateCommand>(*m_manager));

	m_nextInfoButton.setup("Next >>",  { buttonX + 270.f, (windowSize.y * 0.8f) - 35.f}, buttonSize , &m_font);
	m_previousInfoButton.setup("<< Previous", { buttonX - 245.f, (windowSize.y * 0.8f) - 35.f }, buttonSize, &m_font);

 
   

}

void InfoScreenState::render(sf::RenderWindow& window) {
    if (!m_isPositionsSet) {
        setupPositions(window);
        m_isPositionsSet = true;
    }
    sf::Sprite bg;
    bg.setTexture(m_infoImage);
    sf::Vector2u textureSize = m_infoImage.getSize();

    // çéùåá ÷ðä îéãä - úîìà àú ëì äçìåï (âí àí æä îòååú)
    float scaleX = static_cast<float>(WINDOW_WIDTH) / textureSize.x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / textureSize.y;

    bg.setScale(scaleX, scaleY);
    window.draw(bg);
    m_backButton.render(window);
	m_nextInfoButton.draw(window);
	m_previousInfoButton.draw(window);
}

void InfoScreenState::handleInput(sf::Event& event, sf::RenderWindow& window) {
    m_backButton.handleEvent(event, window);
    m_nextInfoButton.setBold(false);
	m_previousInfoButton.setBold(false);
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (m_nextInfoButton.isMouseOver(mousePos)) {
		m_nextInfoButton.setBold(true);
        if (m_nextInfoButton.isMousePressed(mousePos)) {
            m_currentInfoIndex++;
            if (m_currentInfoIndex > 3) { // Assuming there are 3 info screens
                m_currentInfoIndex = 1;
            }
            m_infoImage = ResourceGraphic::getInstance().getTexture("background_info" + std::to_string(m_currentInfoIndex));
       }
       
    } else if (m_previousInfoButton.isMouseOver(mousePos)) {
		m_previousInfoButton.setBold(true);
        if(m_previousInfoButton.isMousePressed(mousePos)) {
            m_currentInfoIndex--;
            if (m_currentInfoIndex < 1) { // Assuming there are 3 info screens
                m_currentInfoIndex = 3;
            }
            m_infoImage = ResourceGraphic::getInstance().getTexture("background_info" + std::to_string(m_currentInfoIndex));
		}
	}
 
}

void InfoScreenState::onExit() {
    // ìåâé÷ä ùøöä áéöéàä (àí öøéê)
}