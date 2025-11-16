#include "AudioManager.h"

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager() : m_currentMusicName("") {


    try {
        loadMusic("menu_music", "BackGroundSound.wav");
        loadMusic("game_music", "level3Music.wav");
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load music paths: " << e.what() << std::endl;
        
    }
	m_isMusicPlaying = true;
}

void AudioManager::loadMusic(const std::string& name, const std::string& filename) {
    m_musicFilepaths[name] = filename;
}

void AudioManager::playMusic(const std::string& name, bool loop, float volume) {
    if (m_currentMusicName == name && m_currentMusic.getStatus() == sf::Music::Playing) {
        return;
    }
    auto it = m_musicFilepaths.find(name);
    if (it == m_musicFilepaths.end()) {
        std::cerr << "Error: Music track not found: " << name << std::endl;
        return;
    }

    const std::string& filename = it->second;
    if (!m_currentMusic.openFromFile(filename)) {
        std::cerr << "Error: Failed to open music file: " << filename << std::endl;
        return;
    }

    m_currentMusic.setLoop(loop);
    m_currentMusic.setVolume(volume);
    m_currentMusic.play();

    m_currentMusicName = name;
	m_isMusicPlaying = true;
    
}

void AudioManager::stopMusic() {
    m_currentMusic.stop();
    m_currentMusicName = "";
	m_isMusicPlaying = false;
}