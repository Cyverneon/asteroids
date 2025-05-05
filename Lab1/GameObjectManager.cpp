#include "GameObjectManager.h"

GameObjectManager& GameObjectManager::getInstance()
{
    static GameObjectManager instance;
    return instance;
}

std::shared_ptr<GameObject> GameObjectManager::createGameObject(
    const std::string& tag,
    const std::string meshTag,
    const std::string shaderTag,
    const std::vector<std::string> textureTags,
    glm::vec3 pos,
    glm::vec3 rot,
    glm::vec3 scale)
{
    if (_gameObjects.find(tag) != _gameObjects.end())
    {
        std::cerr << "GameObject already exists with tag: " << tag << std::endl;
        return nullptr;
    }

    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(tag, meshTag, shaderTag, textureTags, pos, rot, scale);
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

void GameObjectManager::removeGameObject(const std::string& tag)
{
    if (_gameObjects.find(tag) == _gameObjects.end())
    {
        std::cerr << "Cannot remove GameObject. Not found: " << tag << std::endl;
        return;
    }

    _gameObjects.erase(_gameObjects.find(tag));
}

void GameObjectManager::clear()
{
    _gameObjects.clear();
}