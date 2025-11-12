#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

/*
Music manager singleton class to load and play background music using SFML.
*/

class AudioManager {
public:
    static AudioManager& getInstance();

    void loadMusic(const std::string& name, const std::string& filename);

    void playMusic(const std::string& name, bool loop = true, float volume = 50.0f);

    void stopMusic();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

private:
    AudioManager();
    ~AudioManager() = default;

    std::unordered_map<std::string, std::string> m_musicFilepaths;

    sf::Music m_currentMusic;

    std::string m_currentMusicName;
};