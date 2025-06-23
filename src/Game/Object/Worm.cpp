#include "Game/Object/Worm.h"
#include "Game/State/WormIdleState.h"
#include "Game/State/WormAimingState.h"
#include "Game/CollisionCategories.h"
#include "Game/GameController.h"
#include "Game/Object/Weapon/IWeapon.h"
#include <iostream>
#include <cmath>

namespace {
    const float ANIMATION_SPEED_FPS = 12.0f;
}

const sf::Time Worm::ANIMATION_FRAME_DURATION = sf::seconds(1.f / ANIMATION_SPEED_FPS);

Worm::Worm(b2World& world, GameController& controller, const sf::Vector2f& position)
    : m_currentFrame(0),
    m_animationTimer(sf::Time::Zero),
    m_currentAnimation(nullptr),
    m_isFacingRight(true),
    m_jumpsLeft(MAX_JUMPS),
    m_gameController(controller)
{
    if (!m_idleTexture.loadFromFile("wblink1.png")) {
        std::cerr << "Error loading idle texture" << std::endl;
    }
    if (!m_walkTexture.loadFromFile("wwalk.png")) {
        std::cerr << "Error loading walk texture" << std::endl;
    }
    if (!m_jumpTexture.loadFromFile("wjumpu.png")) {
        std::cerr << "Error loading jump texture" << std::endl;
    }
    if (!m_bazookaIdleTexture.loadFromFile("wbaz.png")) {
        std::cerr << "Error loading bazooka idle texture" << std::endl;
    }
    if (!m_bazookaAimTexture.loadFromFile("wbazd.png")) {
        std::cerr << "Error loading bazooka aim texture" << std::endl;
    }

    setupAnimations();

    setAnimation("idle");

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);
    bodyDef.fixedRotation = true;
    m_body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    float boxWidth = m_animations.at("idle")[0].width / SCALE;
	// מכפילים ב0.45 כדי לפצות על הרווח בין התולעת לאדמה
    float boxHeight = (m_animations.at("idle")[0].height * 0.45) / SCALE;
    shape.SetAsBox(boxWidth / 2.0f, boxHeight / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;

    fixtureDef.filter.categoryBits = CATEGORY_WORM;
    fixtureDef.filter.maskBits = CATEGORY_TERRAIN | CATEGORY_PROJECTILE;
    

    m_body->CreateFixture(&fixtureDef);
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    setState(std::make_unique<WormIdleState>());
}

bool Worm::canJump() const { return m_jumpsLeft > 0; }
void Worm::useJump() { if (canJump()) m_jumpsLeft--; }
void Worm::resetJumps() { m_jumpsLeft = MAX_JUMPS; }

bool Worm::isGrounded() {
    for (b2ContactEdge* edge = m_body->GetContactList(); edge; edge = edge->next) {
        if (edge->contact->IsTouching()) {
            b2Fixture* fixtureA = edge->contact->GetFixtureA();
            b2Fixture* fixtureB = edge->contact->GetFixtureB();
            b2Fixture* otherFixture = (fixtureA->GetBody() == m_body) ? fixtureB : fixtureA;
            if (otherFixture->GetFilterData().categoryBits == CATEGORY_TERRAIN) {
                return true;
            }
        }
    }
    return false;
}

GameController& Worm::getGameController() {
    return m_gameController;
}

void Worm::equipWeapon(std::unique_ptr<IWeapon> weapon) {
    setState(std::make_unique<WormAimingState>(std::move(weapon)));
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

int Worm::frameNumber(sf::Texture currTexture) const {
    return currTexture.getSize().y > 0 ? currTexture.getSize().y / currTexture.getSize().x : 1;
}

void Worm::setupAnimations() {
    loadAnimation("idle", m_idleTexture, frameNumber(m_idleTexture));
    loadAnimation("walk", m_walkTexture, frameNumber(m_walkTexture));
    loadAnimation("jump", m_jumpTexture, frameNumber(m_jumpTexture));
    loadAnimation("bazooka_idle", m_bazookaIdleTexture, frameNumber(m_bazookaIdleTexture));
    loadAnimation("bazooka_aim", m_bazookaAimTexture, frameNumber(m_bazookaAimTexture));
}

void Worm::setAnimation(const std::string& name) {
    auto it = m_animations.find(name);
    if (it == m_animations.end()) {
        std::cerr << "Animation not found: " << name << std::endl;
        return;
    }
    const std::vector<sf::IntRect>* nextAnimation = &it->second;
    if (m_currentAnimation == nextAnimation && !name.empty()) return;

    m_currentAnimation = nextAnimation;

    if (name == "idle") m_sprite.setTexture(m_idleTexture, true);
    else if (name == "walk") m_sprite.setTexture(m_walkTexture, true);
    else if (name == "jump") m_sprite.setTexture(m_jumpTexture, true);
    else if (name == "bazooka_idle") m_sprite.setTexture(m_bazookaIdleTexture, true);
    else if (name == "bazooka_aim") m_sprite.setTexture(m_bazookaAimTexture, true);
    else {
        std::cerr << "Unknown animation name for texture setting: " << name << std::endl;
        return;
    }

    m_currentFrame = 0;
    m_animationTimer = sf::Time::Zero;
    if (m_currentAnimation && !m_currentAnimation->empty()) {
        m_sprite.setTextureRect((*m_currentAnimation)[m_currentFrame]);
        m_sprite.setOrigin((*m_currentAnimation)[m_currentFrame].width / 2.f, (*m_currentAnimation)[m_currentFrame].height / 2.f);
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
    if (m_isFacingRight != faceRight) {
        m_isFacingRight = faceRight;
        m_sprite.scale(-1.f, 1.f);
    }
}

void Worm::render(sf::RenderWindow& window) {
    window.draw(m_sprite);
    if (m_state) {
        m_state->render(window, *this);
    }
}

void Worm::update(sf::Time deltaTime) {
    DynamicObject::update(deltaTime);
    updateAnimation(deltaTime);
    if (isGrounded()) {
        resetJumps();
    }
    if (m_state) {
        m_state->update(*this, deltaTime);
    }
}

void Worm::handlePlayerInput(const sf::Event& event) {
    if (m_state) {
        m_state->handleInput(*this, event);
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
    m_body->ApplyLinearImpulse(force, m_body->GetWorldCenter(), true);
}

void Worm::setHorizontalVelocity(float vx) {
    b2Vec2 currentVel = m_body->GetLinearVelocity();
    currentVel.x = vx;
    m_body->SetLinearVelocity(currentVel);
}