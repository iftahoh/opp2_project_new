#pragma once

#include "ICommand.h"
#include "MenuManager.h"

template <typename T>
class PushStateCommand : public ICommand {
public:
    PushStateCommand(MenuManager& manager) : m_manager(manager) {}

    void execute() override {
        // ����� ����� �� ���� ���, ����� ���� ������ �����
        // ����� ������ ��� ����� ����� (T)
        m_manager.pushState<T>();
    }

private:
    MenuManager& m_manager;
};