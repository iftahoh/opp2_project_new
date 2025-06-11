// include/GameObjects/StaticObject.h

#pragma once
#include "GameObject.h"

class StaticObject : public GameObject {
public:
    // הסרת שם הפרמטר תמנע את האזהרה
    void update(sf::Time /*deltaTime*/) override { /* Do nothing */ }
};