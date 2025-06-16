// src/Game/Object/Worm.cpp

#include "Game/Object/Worm.h"
#include "Game/State/WormIdleState.h"
#include "Game/State/WormAimingState.h" // ----- הוספת מצב חדש -----
#include "Game/CollisionCategories.h"
#include "box2d/b2_common.h"
#include <iostream>
#include <cmath>

namespace {
    const float ANIMATION_SPEED_FPS = 12.0f;
}

const sf::Time Worm::ANIMATION_FRAME_DURATION = sf::seconds(1.f / ANIMATION_SPEED_FPS);

Worm::Worm(b2World& world, const sf::Vector2f& position)
    : m_currentFrame(0),
    m_animationTimer(sf::Time::Zero),
    m_currentAnimation(nullptr),
    m_isFacingRight(true),
    m_jumpsLeft(MAX_JUMPS) // ----- אתחול מונה הקפיצות -----
{
    // ... (קוד טעינת טקסטורות והגדרת אנימציות נשאר כפי שהוא) ...
    if (!m_idleTexture.loadFromFile("wblink1.png")) {
        std::cerr << "Error loading idle texture" << std::endl;
    }
    if (!m_walkTexture.loadFromFile("wwalk.png")) {
        std::cerr << "Error loading walk texture" << std::endl;
    }
    if (!m_jumpTexture.loadFromFile("wjumpu.png")) {
        std::cerr << "Error loading jump texture" << std::endl;
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

    fixtureDef.filter.categoryBits = CATEGORY_WORM;
    fixtureDef.filter.maskBits = CATEGORY_TERRAIN;

    m_body->CreateFixture(&fixtureDef);

    setState(std::make_unique<WormIdleState>());
}

// ----- מימוש פונקציות ניהול קפיצה -----
bool Worm::canJump() const {
    return m_jumpsLeft > 0;
}

void Worm::useJump() {
    if (canJump()) {
        m_jumpsLeft--;
    }
}

void Worm::resetJumps() {
    m_jumpsLeft = MAX_JUMPS;
}

bool Worm::isGrounded() {
    // נעבור על כל המגעים של התולעת
    for (b2ContactEdge* edge = m_body->GetContactList(); edge; edge = edge->next) {
        // נוודא שהמגע פעיל ונוגע
        if (edge->contact->IsTouching()) {

            b2Fixture* fixtureA = edge->contact->GetFixtureA();
            b2Fixture* fixtureB = edge->contact->GetFixtureB();

            // נזהה מי מהם הוא ה-fixture של האדמה
            b2Fixture* otherFixture = nullptr;
            if (fixtureA->GetBody() == m_body) {
                otherFixture = fixtureB;
            }
            else {
                otherFixture = fixtureA;
            }

            // אם ה-fixture השני הוא אדמה, התולעת על הקרקע
            if (otherFixture->GetFilterData().categoryBits == CATEGORY_TERRAIN) {
                return true;
            }
        }
    }
    // אם לא מצאנו שום מגע עם האדמה
    return false;
}
// ----- סוף מימוש פונקציות -----

// ... (שאר הפונקציות כמו loadAnimation, setupAnimations, setAnimation, updateAnimation, updateDirection, render נשארות זהות) ...
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

int Worm::frameNumber(sf::Texture currTexture) const
{
    return currTexture.getSize().y / currTexture.getSize().x;
}

void Worm::setupAnimations() {
    loadAnimation("idle", m_idleTexture, frameNumber(m_idleTexture));
    loadAnimation("walk", m_walkTexture, frameNumber(m_walkTexture));
    loadAnimation("jump", m_jumpTexture, frameNumber(m_jumpTexture));
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
    else if (name == "jump") {
        m_sprite.setTexture(m_jumpTexture, true);
    }
    else {
        std::cerr << "Unknown animation name: " << name << std::endl;
        return;
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

    // ----- הוספת איפוס קפיצות בעת נגיעה בקרקע -----
    if (isGrounded()) {
        resetJumps();
    }

    if (m_state) {
        m_state->update(*this, deltaTime);
    }
}

// ----- שינוי חתימה כדי לקבל את האירוע -----
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

void Worm::equipWeapon(std::unique_ptr<IWeapon> weapon) {
    // מעביר את התולעת למצב כיוון עם הנשק שנבחר
    setState(std::make_unique<WormAimingState>(std::move(weapon)));
}