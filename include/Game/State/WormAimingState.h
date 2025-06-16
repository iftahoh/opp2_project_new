#pragma once
#include "Game/State/IWormState.h"
#include "Game/Object/Weapon/IWeapon.h"
#include <memory>

class WormAimingState : public IWormState {
public:
    explicit WormAimingState(std::unique_ptr<IWeapon> weapon);

    void onEnter(Worm& worm) override;
    void handleInput(Worm& worm, const sf::Event& event) override;
    void update(Worm& worm, sf::Time deltaTime) override;
    void render(sf::RenderWindow& window, Worm& worm) override;
    void onExit(Worm& worm) override;

private:
    std::unique_ptr<IWeapon> m_weapon;
};