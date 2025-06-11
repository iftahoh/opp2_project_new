#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <functional>
#include <Menu/ICommand.h>

class Button {
public:
	Button();
	Button(const std::wstring& str, sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size);

	void setup(const std::wstring& str, sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size);
	void setCommand(std::unique_ptr<ICommand> command);
	void handleEvent(const sf::Event& event, sf::RenderWindow& window);
	void render(sf::RenderWindow& window);
	bool isMouseOver(sf::RenderWindow& window);

private:
	sf::RectangleShape m_buttonShape;
	sf::Text m_buttonText;
	sf::Font* font;
	std::unique_ptr<ICommand> m_command;

	sf::Color m_defaultColor = sf::Color(100, 100, 100);
	sf::Color m_hoverColor = sf::Color(150, 150, 150);
	sf::Color m_textColor = sf::Color::White;
};