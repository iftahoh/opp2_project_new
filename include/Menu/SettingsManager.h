#pragma once

#include <string>

/*
SettingsManager class that manages game settings using the singleton pattern.
*/

class SettingsManager {
public:
    // פונקציה סטטית לקבלת המופע היחיד של המחלקה
    static SettingsManager& getInstance();

    // --- הגדרות המשחק ---
    // כרגע נשים רק את מספר התולעים, ונוסיף את השאר בעתיד
    void setWormsPerPlayer(int count);
    int getWormsPerPlayer() const;

    // פונקציות לטעינה ושמירה של ההגדרות
    void saveSettings();
    void loadSettings();

private:
    // בנאי פרטי כדי למנוע יצירה חיצונית
    SettingsManager();

    // מניעת העתקה של הסינגלטון
    SettingsManager(const SettingsManager&) = delete;
    void operator=(const SettingsManager&) = delete;

    // משתנים שיחזיקו את ערכי ההגדרות
    int m_wormsPerPlayer;
    // int m_numberOfPlayers; // נוסיף בעתיד
    // int m_volume;          // נוסיף בעתיד

    const std::string m_settingsFile = "settings.ini";
};