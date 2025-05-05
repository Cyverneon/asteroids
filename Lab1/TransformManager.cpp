#include "TransformManager.h"
#include <iostream>

TransformManager& TransformManager::getInstance()
{
    static TransformManager instance;
    return instance;
}

std::shared_ptr<Transform> TransformManager::addTransform(const std::string& tag, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
    if (_transforms.find(tag) != _transforms.end())
    {
        std::cerr << "Transform already exists for tag: " << tag << std::endl;
        return nullptr;
    }
    std::shared_ptr<Transform> transform = std::make_shared<Transform>(pos, rot, scale);
    _transforms[tag] = transform;
    return transform;
}

std::shared_ptr<Transform> TransformManager::getTransform(const std::string& tag)
{
    if (_transforms.find(tag) == _transforms.end())
    {
        std::cerr << "Error: Transform not found for tag: " << tag << std::endl;
        throw std::runtime_error("Transform not found");
    }
    return _transforms[tag];
}

bool TransformManager::hasTransform(const std::string& tag)
{
    return _transforms.find(tag) != _transforms.end();
}

void TransformManager::removeTransform(const std::string& tag)
{
    if (_transforms.erase(tag) == 0)
    {
        std::cerr << "Warning: Attempted to remove non-existent transform: " << tag << std::endl;
    }
}

void TransformManager::clear()
{
    _transforms.clear();
}
