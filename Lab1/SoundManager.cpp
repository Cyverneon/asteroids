#include "SoundManager.h"

SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}

std::shared_ptr<Sound> SoundManager::loadSound(const std::string& tag, const std::string& file)
{
    if (_sounds.find(tag) != _sounds.end())
    {
        std::cerr << "Sound already exists: " << tag << std::endl;
        return _sounds[tag];
    }

    std::shared_ptr<Sound> sound = std::make_shared<Sound>();
    sound->init(file);
    sound->setupDevice();
    _sounds[tag] = sound;
    
    return sound;
}

std::shared_ptr<Sound> SoundManager::getSound(const std::string& tag)
{
    if (_sounds.find(tag) == _sounds.end())
    {
        std::cerr << "Sound not found" << tag << std::endl;
        return nullptr;
    }

    return _sounds[tag];
}

void SoundManager::playSound(const std::string& tag)
{
    if (_sounds.find(tag) == _sounds.end())
    {
        std::cerr << "Sound not found" << tag << std::endl;
    }

    _sounds[tag]->play();
}

void SoundManager::clear()
{
    _sounds.clear();
}