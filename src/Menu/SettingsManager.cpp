#include "Menu/SettingsManager.h"
#include "Menu/MenuManager.h"
#include <fstream>
#include <iostream>

// מימוש הגישה לסינגלטון
SettingsManager& SettingsManager::getInstance() {
    static SettingsManager instance;
    return instance;
}

// בנאי: קובע את ערכי ברירת המחדל
SettingsManager::SettingsManager() : m_wormsPerPlayer(3) {
}


void SettingsManager::setWormsPerPlayer(int count) {
    // בדיקת תקינות בסיסית
    if (count > 2 && count <= 6) {
        m_wormsPerPlayer = count;
    }
}

int SettingsManager::getWormsPerPlayer() const {
    return m_wormsPerPlayer;
}

void SettingsManager::saveSettings() {
    /*std::ofstream file(m_settingsFile);
    if (file.is_open()) {
        file << "worms=" << m_wormsPerPlayer << std::endl;
        file.close();
    }*/
}

void SettingsManager::loadSettings() {
    std::ifstream file(m_settingsFile);
    if (!file.is_open()) {
        return; // אם הקובץ לא קיים, פשוט נשתמש בערכי ברירת המחדל
    }

    std::string line;
    while (getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            if (key == "worms") {
                try {
                    // קורא את הערך מהקובץ ומעדכן את המשתנה
                    setWormsPerPlayer(std::stoi(line.substr(pos + 1)));
                }
                catch (const std::exception& e) {
                    std::cerr << "Error reading settings file: " << e.what() << std::endl;
                }
            }
        }
    }
    file.close();
}