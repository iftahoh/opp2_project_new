#include "Menu/MainMenuScreenState.h"
#include "Menu/MenuManager.h"
#include "Menu/StartGameCommand.h"
#include "Menu/ExitCommand.h"
#include "Menu/PushStateCommand.h"
#include "Menu/InfoScreenState.h"
#include "Menu/SettingsState.h"
#include <iostream>
#include <stdexcept>
#include "ResourceGraphic.h"
#include "AudioManager.h"

MainMenuScreenState::MainMenuScreenState() : m_isPositionsSet(false) {
	try {
		font = ResourceGraphic::getInstance().getFont("main_font");
		backgroundTexture = ResourceGraphic::getInstance().getTexture("background");
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	background.setTexture(backgroundTexture);
}

void MainMenuScreenState::setupPositions(sf::RenderWindow& window) {
	sf::Vector2f windowSize(window.getSize());
	float centerX = windowSize.x / 2.f;

	background.setScale(
		windowSize.x / background.getLocalBounds().width,
		windowSize.y / background.getLocalBounds().height
	);

	sf::Vector2f buttonSize(250.f, 50.f);
	float buttonX = centerX - (buttonSize.x / 2.f);

	startGameButton.setup(L"Start Game", font, { buttonX, windowSize.y * 0.40f }, buttonSize);
	startGameButton.setCommand(std::make_unique<StartGameCommand>(window));

	settingsButton.setup(L"Settings", font, { buttonX, windowSize.y * 0.50f }, buttonSize);
	settingsButton.setCommand(std::make_unique<PushStateCommand<SettingsState>>(*m_manager));

	infoButton.setup(L"Info", font, { buttonX, windowSize.y * 0.60f }, buttonSize);
	infoButton.setCommand(std::make_unique<PushStateCommand<InfoScreenState>>(*m_manager));

	exitGameButton.setup(L"Exit", font, { buttonX, windowSize.y * 0.70f }, buttonSize);
	exitGameButton.setCommand(std::make_unique<ExitCommand>(window));
}

void MainMenuScreenState::render(sf::RenderWindow& window) {
	if (!m_isPositionsSet) {
		setupPositions(window);
		m_isPositionsSet = true;
	}
	if (AudioManager::getInstance().isMusicPlaying()) {
		AudioManager::getInstance().playMusic("menu_music");
	}
	window.draw(background);
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
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M) {
		if(AudioManager::getInstance().isMusicPlaying()) {
			AudioManager::getInstance().stopMusic();
		} else {
			AudioManager::getInstance().playMusic("menu_music");
		}
	}
}

void MainMenuScreenState::onEnter(MenuManager* manager) {

	MenuScreenState::onEnter(manager);
}

void MainMenuScreenState::onExit() {
	
}