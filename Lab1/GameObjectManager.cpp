#include "GameObjectManager.h"

GameObjectManager& GameObjectManager::getInstance()
{
    static GameObjectManager instance;
    return instance;
}

std::shared_ptr<GameObject> GameObjectManager::createGameObject(const std::string& tag, const std::string meshTag, const std::string shaderTag)
{
    if (_gameObjects.find(tag) != _gameObjects.end())
    {
        std::cerr << "GameObject already exists with tag: " << tag << std::endl;
        return _gameObjects[tag];
    }

    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(tag, meshTag, shaderTag);
    _gameObjects[tag] = gameObject;
    return gameObject;
}

std::shared_ptr<GameObject> GameObjectManager::getGameObject(const std::string& tag)
{
    if (_gameObjects.find(tag) == _gameObjects.end())
    {
        std::cerr << "GameObject not found: " << tag << std::endl;
        return nullptr;
    }

    return _gameObjects[tag];
}

void GameObjectManager::clear()
{
    _gameObjects.clear();
}