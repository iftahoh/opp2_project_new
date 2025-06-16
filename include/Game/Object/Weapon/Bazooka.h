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

private:
    void fire(Worm& worm);

    sf::Sprite m_crosshairSprite;
    sf::Texture& m_crosshairTexture;
};