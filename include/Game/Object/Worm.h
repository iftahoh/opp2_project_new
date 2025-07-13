// include/Game/Object/Worm.h

#pragma once

#include "Game/Object/DynamicObject.h"
#include "Game/State/IWormState.h"
#include <memory>
#include <vector>
#include <string>
#include <map>

class GameController;
class IWeapon;

class Worm : public DynamicObject {
public:
    Worm(b2World& world, GameController& controller, const sf::Vector2f& position);

    void render(sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;

    void handlePlayerInput(const sf::Event& event);
    void setState(std::unique_ptr<IWormState> newState);

    void setAnimation(const std::string& name);
    void applyForce(const b2Vec2& force);
    void updateDirection(bool faceRight);
    void setHorizontalVelocity(float vx);

    bool canJump() const;
    void useJump();
    void resetJumps();
    bool isGrounded();

    void equipWeapon(std::unique_ptr<IWeapon> weapon);
    GameController& getGameController();
    const sf::Sprite& getSprite() const { return m_sprite; }

    // --- הוספנו פונקציה להורדת חיים ---
    void takeDamage(int amount);

private:
    void setupAnimations();
    void updateAnimation(sf::Time deltaTime);
    void loadAnimation(const std::string& name, const sf::Texture& texture, int frameCount);
    int frameNumber(sf::Texture currTexture) const;

    // --- הוספנו פונקציה לעדכון מד החיים ---
    void updateHealthBar();

    GameController& m_gameController;
    std::unique_ptr<IWormState> m_state;

    bool m_isFacingRight;
    int m_jumpsLeft;
    static const int MAX_JUMPS = 2;

    // --- הוספנו משתנים למד החיים ---
    int m_health;
    static const int MAX_HEALTH = 100;
    sf::RectangleShape m_healthBarBackground;
    sf::RectangleShape m_healthBarForeground;
    // ------------------------------------

    std::map<std::string, std::vector<sf::IntRect>> m_animations;
    const std::vector<sf::IntRect>* m_currentAnimation;

    int m_currentFrame;
    sf::Time m_animationTimer;
    static const sf::Time ANIMATION_FRAME_DURATION;

    sf::Texture m_idleTexture;
    sf::Texture m_walkTexture;
    sf::Texture m_jumpTexture;
    sf::Texture m_bazookaIdleTexture;
    sf::Texture m_bazookaAimTexture;

    static constexpr float SCALE = 30.0f;
};