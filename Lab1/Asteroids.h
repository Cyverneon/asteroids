#pragma once
#include "Game.h"
#include "Mesh.h"
#include "Texture.h"

class Asteroids : public Game
{
public: 
	Asteroids();
	~Asteroids();

	//initialise
	void loadPhysicsEngine();
	void initPlayer();

	//run
	void createAsteroids();
	void movePlayer(float delta);

	// override functions
	void loadMeshes() override;
	void loadShaders() override;
	void loadTextures() override;
	void initialiseGame() override;
	void updateGame(float delta) override;

private:
	std::shared_ptr<GameObject> _player;
	Transform* _playerTransform = nullptr;

	// units per second
	float _playerSpeed = 50.0f;
	// degrees per second
	float _playerRotSpeed = 180.0f;

	// Function pointers for physics engine functions
	void (*setForwardDirection)(GameObject*, glm::vec3) = nullptr;
	void (*applyThrust)(GameObject*, float) = nullptr;
	void (*updatePhysics)(GameObject*, float) = nullptr;
	bool (*checkCollisionRadius)(const GameObject*, const GameObject*, float, float) = nullptr;
	bool (*checkCollisionAABB)(const GameObject*, const GameObject*, const glm::vec3&, const glm::vec3&) = nullptr;
};

