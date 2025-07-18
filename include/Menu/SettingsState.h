#pragma once

#include "MenuScreenState.h"
#include "Button.h"
#include <SFML/Graphics/Text.hpp> // הוספת ה-include הדרוש לטקסט

class SettingsState : public MenuScreenState {
public:
    SettingsState();

    void onEnter(MenuManager* manager) override;
    void render(sf::RenderWindow& window) override;
    void handleInput(sf::Event& event, sf::RenderWindow& window) override;
    void onExit() override;

private:
    void setupPositions(const sf::RenderWindow& window);
    void updateTextValues(); // פונקציית העזר לסנכרון התצוגה

	sf::Font m_textFont; // הוספת משתנה לשמירת הגופן
	sf::Font m_numberFont; // גופן נוסף למספרים
    sf::Texture m_settingsImage;
    Button m_backButton;

    bool m_isPositionsSet;

    // --- פקדים לשליטה על מספר התולעים ---
    sf::Text m_wormsLabel;       // התווית הקבועה "Worms"
    sf::Text m_wormsValueText;   // הטקסט שמציג את המספר המשתנה
    Button m_wormsUpButton;      // כפתור +
    Button m_wormsDownButton;    // כפתור -
};