#include "Game/Object/Worm.h"
#include "Game/State/WormIdleState.h"
#include "box2d/b2_common.h"
#include <iostream>
#include <cmath>

namespace {
    const int IDLE_FRAME_COUNT = 6;
    const int WALK_FRAME_COUNT = 15;
    const float ANIMATION_SPEED_FPS = 12.0f;
}

const sf::Time Worm::ANIMATION_FRAME_DURATION = sf::seconds(1.f / ANIMATION_SPEED_FPS);

Worm::Worm(b2World& world, const sf::Vector2f& position)
    : m_currentFrame(0),
    m_animationTimer(sf::Time::Zero),
    m_currentAnimation(nullptr),
    m_isFacingRight(true)
{
    if (!m_idleTexture.loadFromFile("wblink1.png")) {
        std::cerr << "Error loading idle texture" << std::endl;
    }
    if (!m_walkTexture.loadFromFile("wwalk.png")) {
        std::cerr << "Error loading walk texture" << std::endl;
    }

    setupAnimations();

    setAnimation("idle");
    m_sprite.setOrigin(m_animations.at("idle")[0].width / 2.f, m_animations.at("idle")[0].height / 2.f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);
    bodyDef.fixedRotation = true;
    m_body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    float boxWidth = m_animations.at("idle")[0].width / SCALE;
    float boxHeight = m_animations.at("idle")[0].height / SCALE;
    shape.SetAsBox(boxWidth / 2.0f, boxHeight / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    m_body->CreateFixture(&fixtureDef);

    setState(std::make_unique<WormIdleState>());
}

void Worm::loadAnimation(const std::string& name, const sf::Texture& texture, int frameCount) {
    std::vector<sf::IntRect> frames;
    sf::Vector2u textureSize = texture.getSize();
    int frameWidth = textureSize.x;
    int frameHeight = textureSize.y / frameCount;

    for (int i = 0; i < frameCount; ++i) {
        frames.push_back(sf::IntRect(0, i * frameHeight, frameWidth, frameHeight));
    }
    m_animations[name] = frames;
}

void Worm::setupAnimations() {
    loadAnimation("idle", m_idleTexture, IDLE_FRAME_COUNT);
    loadAnimation("walk", m_walkTexture, WALK_FRAME_COUNT);
}

void Worm::setAnimation(const std::string& name) {
    auto it = m_animations.find(name);
    if (it == m_animations.end()) {
        std::cerr << "Animation not found: " << name << std::endl;
        return;
    }
    const std::vector<sf::IntRect>* nextAnimation = &it->second;
    if (m_currentAnimation == nextAnimation) return;

    m_currentAnimation = nextAnimation;

    if (name == "idle") {
        m_sprite.setTexture(m_idleTexture, true);
    }
    else if (name == "walk") {
        m_sprite.setTexture(m_walkTexture, true);
    }

    m_currentFrame = 0;
    m_animationTimer = sf::Time::Zero;
    if (!m_currentAnimation->empty()) {
        m_sprite.setTextureRect((*m_currentAnimation)[m_currentFrame]);
    }
}

void Worm::updateAnimation(sf::Time deltaTime) {
    if (!m_currentAnimation || m_currentAnimation->empty()) return;

    m_animationTimer += deltaTime;
    if (m_animationTimer >= ANIMATION_FRAME_DURATION) {
        m_animationTimer -= ANIMATION_FRAME_DURATION;
        m_currentFrame = (m_currentFrame + 1) % m_currentAnimation->size();
        m_sprite.setTextureRect((*m_currentAnimation)[m_currentFrame]);
    }
}

void Worm::updateDirection(bool faceRight) {
    if (m_isFacingRight == faceRight) return;
    m_isFacingRight = faceRight;
    sf::Vector2f currentScale = m_sprite.getScale();
    m_sprite.setScale(-currentScale.x, currentScale.y);
}

void Worm::render(sf::RenderWindow& window) {
    sf::Vector2f currentPos = m_sprite.getPosition();
    const float y_offset_pixels = 19.0f;
    m_sprite.setPosition(currentPos.x, currentPos.y + y_offset_pixels);
    window.draw(m_sprite);
    m_sprite.setPosition(currentPos);
}

void Worm::update(sf::Time deltaTime) {
    DynamicObject::update(deltaTime);
    updateAnimation(deltaTime);
    if (m_state) {
        m_state->update(*this, deltaTime);
    }
}

void Worm::handlePlayerInput() {
    if (m_state) {
        m_state->handleInput(*this);
    }
}

void Worm::setState(std::unique_ptr<IWormState> newState) {
    if (m_state) {
        m_state->onExit(*this);
    }
    m_state = std::move(newState);
    if (m_state) {
        m_state->onEnter(*this);
    }
}

void Worm::applyForce(const b2Vec2& force) {
    m_body->ApplyForceToCenter(force, true);
}