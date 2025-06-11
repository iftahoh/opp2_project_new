#pragma once

#include "Game/Object/GameObject.h"

class DynamicObject : public GameObject {
public:
    // update יעודכן כך שיסנכרן את הגרפיקה עם הפיזיקה
    void update(sf::Time deltaTime) override;
};