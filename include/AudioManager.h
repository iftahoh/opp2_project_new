#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

class AudioManager {
public:
    // פונקציית הגישה לסינגלטון
    static AudioManager& getInstance();

    // טוען נתיך של קובץ מוזיקה
    void loadMusic(const std::string& name, const std::string& filename);

    // מנגן מוזיקה לפי שם
    // עוצר אוטומטית מוזיקה קודמת שמתנגנת
    void playMusic(const std::string& name, bool loop = true, float volume = 50.0f);

    // עוצר את המוזיקה הנוכחית
    void stopMusic();

    // מונע העתקה (חלק מעיצוב הסינגלטון)
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

private:
    AudioManager(); // בנאי פרטי
    ~AudioManager() = default;

    // מפה שמחזיקה את הנתיבים לקבצי המוזיקה
    std::unordered_map<std::string, std::string> m_musicFilepaths;

    // אובייקט המוזיקה של SFML.
    // נחזיק רק אחד כי בדרך כלל רק רצועת רקע אחת מתנגנת
    sf::Music m_currentMusic;

    // שם הרצועה שמתנגנת כרגע
    std::string m_currentMusicName;
};