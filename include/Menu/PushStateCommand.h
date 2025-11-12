#pragma once

#include "ICommand.h"
#include "MenuManager.h"

/*
PushStateCommand class that implements the ICommand interface 
to push a new menu state onto the MenuManager's state stack.
*/

template <typename T>
class PushStateCommand : public ICommand {
public:
    PushStateCommand(MenuManager& manager) : m_manager(manager) {}

    void execute() override {
        m_manager.pushState<T>();
    }

private:
    MenuManager& m_manager;
};