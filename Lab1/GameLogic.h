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
	void spawnBullet();
	void spawnAsteroid(std::string tag, glm::vec3 pos, glm::vec3 rot);
	void spawnAsteroidRound();
	void processInput(float delta);
	void movePlayer(float delta);
	void moveAsteroids(float delta);
	void moveBullets(float delta);
	glm::vec3 wrapObjectPosition(glm::vec3 pos);

	const Uint8* _kbState;

	std::shared_ptr<GameObject> _player;

	std::vector<std::shared_ptr<GameObject>> _asteroids;

	std::vector<std::string> _playerBullets;

	// units per second
	const float _playerSpeed = 40.0f;
	// degrees per second
	const float _playerRotSpeed = 180.0f;

	bool _bulletFired = false;

	const float _asteroidSpeed = 2.0f;

	const float _bulletSpeed = 50.0f;;

	// object boundaries in world space
	// since the origin is (0, 0, 0), gameplay code assumes the min offsets are max offsets * -1
	// things would need fixed if the origin wasn't 0 on all axis but idk any reason to not keep it simple and do that
	const float maxX = 12.8;
	const float maxZ = 7.3;

	// Function pointers for physics engine functions
	void (*setForwardDirection)(GameObject*, glm::vec3) = nullptr;
	void (*setForwardDirectionFromRot)(GameObject*, glm::vec3) = nullptr;
	void (*applyThrust)(GameObject*, float) = nullptr;
	void (*updatePhysics)(GameObject*, float) = nullptr;
	bool (*checkCollisionRadius)(const GameObject*, const GameObject*, float, float) = nullptr;
	bool (*checkCollisionAABB)(const GameObject*, const GameObject*, const glm::vec3&, const glm::vec3&) = nullptr;
};

