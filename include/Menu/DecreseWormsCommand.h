#pragma once
#include "Menu/ICommand.h"
#include "Menu/SettingsManager.h"

/*
* DecreseWormsCommand class that implements the ICommand interface
*/

class DecreseWormsCommand : public ICommand {
public:
    void execute() override {
        auto& settings = SettingsManager::getInstance();
        int currentWorms = settings.getWormsPerPlayer();
        settings.setWormsPerPlayer(currentWorms - 1);
    }
};