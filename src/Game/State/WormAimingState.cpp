#include "Game/State/WormAimingState.h"
#include "Game/Object/Worm.h"
#include "Game/GameController.h" // ייתכן שנצטרך גישה לבקר המשחק

WormAimingState::WormAimingState(std::unique_ptr<IWeapon> weapon)
    : m_weapon(std::move(weapon)) {
}

void WormAimingState::onEnter(Worm& worm) {
    // אולי נרצה לשנות את האנימציה של התולעת למצב "מכוון"
    // worm.setAnimation("aiming");
}

void WormAimingState::handleInput(Worm& worm, const sf::Event& event) {
    if (m_weapon) {
        // העבר את הטיפול בקלט לנשק עצמו
        m_weapon->handleInput(event, worm);
    }
}

void WormAimingState::update(Worm& worm, sf::Time deltaTime) {
    // עדכן את הנשק (למשל, את מיקום הכוונת)
    // נצטרך דרך להשיג את בקר המשחק כדי להעביר אותו לנשק
    // worm.getGameController() - פונקציה שצריך להוסיף
    // if (m_weapon) {
    //     m_weapon->update(worm, worm.getGameController());
    // }
}

void WormAimingState::onExit(Worm& worm) {
    // הפעולה שתתבצע כשהתולעת יוצאת ממצב כיוון (למשל, אחרי ירייה)
}