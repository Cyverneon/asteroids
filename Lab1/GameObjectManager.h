#pragma once
#include <unordered_map>
#include <memory>
#include <iostream>
#include "GameObject.h"

class GameObjectManager
{
public:
	static GameObjectManager& getInstance();

	std::shared_ptr<GameObject> createGameObject(const std::string& tag, Transform* transform, Mesh* mesh, Shader* shader);
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

