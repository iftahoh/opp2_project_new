#pragma once

#include "Game/Object/GameObject.h"

class DynamicObject : public GameObject {
public:
    // update ������ �� ������� �� ������� �� �������
    void update(sf::Time deltaTime) override;
};