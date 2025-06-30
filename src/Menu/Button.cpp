#include <Menu/Button.h>
#include <SFML/Graphics.hpp>
#include <iostream>
//------------------------------------------------------------------
Button::Button() : font(nullptr) {}

//------------------------------------------------------------------
Button::Button(const std::wstring& str, sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size) : font(&font){
	setup(str, font, position, size);
}

//------------------------------------------------------------------
void Button::setup(const std::wstring& str, sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size) {
	this->font = &font;
	m_buttonShape.setSize(size);
	m_buttonShape.setPosition(position);
	m_buttonShape.setFillColor(m_defaultColor);
	m_buttonShape.setOutlineThickness(2.f);
	m_buttonShape.setOutlineColor(sf::Color::Black);

	m_buttonText.setFont(*this->font);
	m_buttonText.setString(str);
	m_buttonText.setFillColor(m_textColor);
	m_buttonText.setCharacterSize(static_cast<unsigned int> (size.y * 0.6f));
	
	sf::FloatRect textBounds = m_buttonText.getLocalBounds();
	m_buttonText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
	m_buttonText.setPosition(position.x + size.x / 2.f, position.y + size.y / 2.f);
}

//------------------------------------------------------------------
void Button::setCommand(std::unique_ptr<ICommand> command) {
	m_command = std::move(command);
}

//------------------------------------------------------------------
bool Button::isMouseOver(sf::RenderWindow& window) {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);
	return m_buttonShape.getGlobalBounds().contains(worldPos);
}

sf::Vector2f Button::getposition() const
{
	return m_buttonShape.getPosition();
}

//------------------------------------------------------------------
void Button::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
	if (isMouseOver(window)) {
		m_buttonShape.setFillColor(m_hoverColor);
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

			if (m_command) {
				std::cout << "LEFT CLICK detected on " << m_buttonText.getString().toAnsiString() << std::endl;
				m_command->execute();
			}
		}
	}
	else {
		m_buttonShape.setFillColor(m_defaultColor);
	}
}

//------------------------------------------------------------------
void Button::render(sf::RenderWindow& window) {
	window.draw(m_buttonShape);
	window.draw(m_buttonText);
}