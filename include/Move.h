#pragma once 

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
/*
Move class in charge of the mouse action in menus.
*/
class Move {

public:
	Move(const std::string& name, const sf::Vector2f& position, const sf::Vector2f& size, sf::Font* font) {
		setup(name, position, size, font);
	}
	Move() = default;
	void draw(sf::RenderWindow& window) {
		window.draw(m_shape);
		window.draw(m_text);
	}
	bool isMouseOver(const sf::Vector2f& mousePosition) const {
		return m_text.getGlobalBounds().contains(mousePosition);
	}
	bool isMousePressed(const sf::Vector2f& mousePosition) const {
		return isMouseOver(mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}
	void setBold(bool bold) {
			m_text.setStyle(bold ? sf::Text::Bold : sf::Text::Regular);
	}
	void setup(const std::string& name, const sf::Vector2f& position, const sf::Vector2f& size, sf::Font* font) {
		m_shape.setPosition(position);
		m_shape.setSize(size);
		m_shape.setFillColor(m_defaultColor);
		m_text.setFont(*font);
		m_text.setString(name);
		m_text.setCharacterSize(30);
		m_text.setFillColor(m_textColor);
		m_text.setPosition(position.x + 10, position.y + 10);
		m_font = font;
	}

private:
	sf::RectangleShape m_shape;
	sf::Text m_text;
	sf::Font* m_font;

	sf::Color m_defaultColor = sf::Color(255,0,0,0);
	sf::Color m_hoverColor = sf::Color(150, 150, 150);
	sf::Color m_textColor = sf::Color::White;
};