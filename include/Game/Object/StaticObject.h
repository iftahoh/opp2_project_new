// include/GameObjects/StaticObject.h

#pragma once
#include "GameObject.h"

class StaticObject : public GameObject {
public:
    // ���� �� ������ ���� �� ������
    void update(sf::Time /*deltaTime*/) override { /* Do nothing */ }
};