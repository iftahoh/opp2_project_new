#pragma once
#include "Menu/ICommand.h" // ודא שיש לך את ה-include הנכון
#include "Menu/SettingsManager.h"

class IncreaseWormsCommand : public ICommand {
public:
    // בדרך כלל ב-ICommand יש פונקציית execute
    void execute() override {
        auto& settings = SettingsManager::getInstance();
        int currentWorms = settings.getWormsPerPlayer();
        settings.setWormsPerPlayer(currentWorms + 1);
    }
};