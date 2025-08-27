#include "Game/State/WormGraveState.h"
#include "Game/Object/Worm.h"
#include "ResourceGraphic.h"
#include <box2d/box2d.h>

void WormGraveState::onEnter(Worm& worm) {
    // פיזיקה: נעצור את הגוף וננטרל אותו
    //if (auto* body = worm.getBody()) {
    //    body->SetLinearVelocity(b2Vec2_zero);
    //    body->SetAngularVelocity(0.f);
    //    body->SetType(b2_staticBody);
    //    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
    //        f->SetSensor(true); // שלא יחסום/ייפגע
    //    }
    //}
}

void WormGraveState::handleInput(Worm& worm, const sf::Event& event) {
    // מת לגמרי — אין קלט
}

void WormGraveState::update(Worm& worm, sf::Time deltaTime) {
    // מת לגמרי — לא זז
}

void WormGraveState::onExit(Worm& worm) {
    // GRAVE הוא מצב סופי — לא אמור לצאת ממנו
}
