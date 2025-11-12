#pragma once
#include "Menu/ICommand.h"
#include "Menu/SettingsManager.h"

/*
IncreaseWormsCommand class that implements the ICommand interface
*/

class IncreaseWormsCommand : public ICommand {
public:
    void execute() override {
        auto& settings = SettingsManager::getInstance();
        int currentWorms = settings.getWormsPerPlayer();
        settings.setWormsPerPlayer(currentWorms + 1);
    }
};