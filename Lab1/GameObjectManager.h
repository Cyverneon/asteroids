#pragma once
#include <unordered_map>
#include <memory>
#include <iostream>
#include "GameObject.h"
#include "TransformManager.h"

class GameObjectManager
{
public:
	static GameObjectManager& getInstance();

	std::shared_ptr<GameObject> createGameObject(
		const std::string& tag,
		const std::string meshTag,
		const std::string shaderTag,
		const std::vector<std::string> textureTags,
		glm::vec3 pos = glm::vec3(0, 0, 0),
		glm::vec3 rot = glm::vec3(0, 0, 0),
		glm::vec3 scale = glm::vec3(1, 1, 1));
	std::shared_ptr<GameObject> getGameObject(const std::string& tag);

	std::unordered_map<std::string, std::shared_ptr<GameObject>> getGameObjects() { return _gameObjects; }

	void clear();

private:
	GameObjectManager() = default;
	~GameObjectManager() = default;
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;

	std::unordered_map<std::string, std::shared_ptr<GameObject>> _gameObjects;
};

