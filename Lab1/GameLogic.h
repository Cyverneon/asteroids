#pragma once
#include "GameObject.h"
#include "GameObjectManager.h"
#include "UBOManager.h"
#include "DLLManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "SoundManager.h"
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
	void initPlayer();

	//run
	void spawnBullet();
	void spawnAsteroid(std::string size, glm::vec3 pos, glm::vec3 rot);
	void spawnAsteroidRound();
	void processInput(float delta);

	void movePlayer(float delta);

	void updateAsteroids(float delta);
	void updateBullets(float delta);

	void checkPlayerDeath();

	const Uint8* _kbState;

	std::shared_ptr<GameObject> _player;

	std::vector<std::string> _asteroids;

	std::vector<std::string> _playerBullets;

	bool _bulletFired = false;

	int _round = 1;

	// configuration
	
	const int _max_asteroids = 6;

	// units per second
	const float _playerSpeed = 40.0f;
	// degrees per second
	const float _playerRotSpeed = 180.0f;

	const float _asteroidSpeed = 2.0f;
	const std::unordered_map<std::string, float> _asteroidSizes =
	{
		{"large", 1.5},
		{"medium", 1.0},
		{"small", 0.5}
	};

	const float _bulletSpeed = 50.0f;;

	// object boundaries in world space
	// since the origin is (0, 0, 0), gameplay code assumes the min offsets are max offsets * -1
	// things would need fixed if the origin wasn't 0 on all axes
	const float _maxX = 12.8;
	const float _maxZ = 7.3;
};

