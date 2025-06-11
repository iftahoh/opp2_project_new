#pragma once

#include "ICommand.h"
#include "MenuManager.h"

class PopStateCommand : public ICommand {
public:
    PopStateCommand(MenuManager& manager) : m_manager(manager) {}

    void execute() override {
        m_manager.popState();
    }
private:
    MenuManager& m_manager;
};