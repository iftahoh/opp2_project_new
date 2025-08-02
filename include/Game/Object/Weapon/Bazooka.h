#pragma once
#include "Game/Object/Weapon/IWeapon.h"
#include "SFML/Graphics.hpp"
#include "ResourceGraphic.h"

class Bazooka : public IWeapon {
public:
    Bazooka();
    void handleInput(const sf::Event& event, Worm& worm) override;
    void update(Worm& worm, GameController& controller) override;
    void render(sf::RenderWindow& window, const Worm& worm) override;

    // ✅ פונקציות חדשות
    std::string getAimingAnimationName() const override;
    std::string getIdleAnimationName() const override;

private:
    void fire(Worm& worm);

    sf::Sprite m_crosshairSprite;
    sf::Texture& m_crosshairTexture;
	sf::Vector2f m_lastMousePos; // îé÷åí äòëáø äàçøåï
};