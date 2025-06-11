#include "Menu/MainMenuScreenState.h"
#include "Menu/MenuManager.h"
#include "Menu/StartGameCommand.h"
#include "Menu/ExitCommand.h"
#include "Menu/PushStateCommand.h"
#include "Menu/InfoScreenState.h"
#include <iostream>
#include <stdexcept>
#include "ResourceGraphic.h"

// �����, ���� �� ������ ����� �� ����
MainMenuScreenState::MainMenuScreenState() : m_isPositionsSet(false) {
	try {
		font = ResourceGraphic::getInstance().getFont("main_font");
		backgroundTexture = ResourceGraphic::getInstance().getTexture("background");
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	// ������ �������� ��� �����
	background.setTexture(backgroundTexture);
	titleText.setFont(font);
	titleText.setString(L"Worms Game");
	titleText.setCharacterSize(52);
	titleText.setFillColor(sf::Color::Black);
	titleText.setStyle(sf::Text::Bold);
}

// �������� ���� ����� ������� ���
void MainMenuScreenState::setupPositions(sf::RenderWindow& window) {
	sf::Vector2f windowSize(window.getSize());
	float centerX = windowSize.x / 2.f;

	// ����� ����
	background.setScale(
		windowSize.x / background.getLocalBounds().width,
		windowSize.y / background.getLocalBounds().height
	);

	// ����� ������
	sf::FloatRect titleBounds = titleText.getLocalBounds();
	titleText.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
	titleText.setPosition(centerX, windowSize.y * 0.15f);

	// ������ ������ ��������
	sf::Vector2f buttonSize(250.f, 50.f);
	float buttonX = centerX - (buttonSize.x / 2.f);

	// ����� ��������
	startGameButton.setup(L"Start Game", font, { buttonX, windowSize.y * 0.40f }, buttonSize);
	startGameButton.setCommand(std::make_unique<StartGameCommand>(window));

	settingsButton.setup(L"Settings", font, { buttonX, windowSize.y * 0.50f }, buttonSize);
	// settingsButton.setCommand(...);

	infoButton.setup(L"Info", font, { buttonX, windowSize.y * 0.60f }, buttonSize);
	infoButton.setCommand(std::make_unique<PushStateCommand<InfoScreenState>>(*m_manager));

	exitGameButton.setup(L"Exit", font, { buttonX, windowSize.y * 0.70f }, buttonSize);
	exitGameButton.setCommand(std::make_unique<ExitCommand>(window));
}

void MainMenuScreenState::render(sf::RenderWindow& window) {
	if (!m_isPositionsSet) {
		setupPositions(window); // ����� ������ �����
		m_isPositionsSet = true;
	}

	// ����� ����� ���
	window.draw(background);
	window.draw(titleText);
	startGameButton.render(window);
	settingsButton.render(window);
	infoButton.render(window);
	exitGameButton.render(window);
}

void MainMenuScreenState::handleInput(sf::Event& event, sf::RenderWindow& window) {
	startGameButton.handleEvent(event, window);
	settingsButton.handleEvent(event, window);
	infoButton.handleEvent(event, window);
	exitGameButton.handleEvent(event, window);
}

void MainMenuScreenState::onEnter(MenuManager* manager) {
	// ����� ������ �������� �� ����� ���
	// �� ����� ������� m_manager ���� �� ���� �����
	MenuScreenState::onEnter(manager);
}

void MainMenuScreenState::onExit() {
	// ������ ���� ������ �����
}