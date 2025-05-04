#pragma once
#include "GameObject.h"
#include "GameObjectManager.h"
#include "UBOManager.h"
#include "DLLManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

	void initialiseGame();
	void initCamera(float width, float height);
	void updateGame(float delta);

	Camera _camera;

private:
	//initialise
	void loadPhysicsEngine();
	void initPlayer();

	//run
	void createAsteroids();
	void movePlayer(float delta);
	void moveAsteroids(float delta);
	glm::vec3 wrapObjectPosition(glm::vec3 pos);

	std::shared_ptr<GameObject> _player;
	Transform* _playerTransform = nullptr;

	std::vector<std::shared_ptr<GameObject>> _asteroids;

	// units per second
	float _playerSpeed = 50.0f;
	// degrees per second
	float _playerRotSpeed = 180.0f;

	float _asteroidSpeed = 3.0;

	// object boundaries in world space
	float minX = -12.8;
	float maxX = 12.8;
	float minZ = -7.3;
	float maxZ = 7.3;

	// Function pointers for physics engine functions
	void (*setForwardDirection)(GameObject*, glm::vec3) = nullptr;
	void (*setForwardDirectionFromRot)(GameObject*, glm::vec3) = nullptr;
	void (*applyThrust)(GameObject*, float) = nullptr;
	void (*updatePhysics)(GameObject*, float) = nullptr;
	bool (*checkCollisionRadius)(const GameObject*, const GameObject*, float, float) = nullptr;
	bool (*checkCollisionAABB)(const GameObject*, const GameObject*, const glm::vec3&, const glm::vec3&) = nullptr;
};

