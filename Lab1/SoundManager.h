#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Sound.h"

class SoundManager
{
public:
    static SoundManager& getInstance(); // Singleton instance

    std::shared_ptr<Sound> loadSound(const std::string& tag, const std::string& file);
    std::shared_ptr<Sound> getSound(const std::string& tag);
    void playSound(const std::string& tag);

    void clear();

private:
    SoundManager() = default;  // Private constructor (Singleton)
    ~SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<Sound>> _sounds;
};

