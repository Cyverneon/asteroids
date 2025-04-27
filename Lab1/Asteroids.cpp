#include "Asteroids.h"

Asteroids::Asteroids()
{

}

Asteroids::~Asteroids()
{

}

void Asteroids::initialiseGame()
{
	loadPhysicsEngine();
	initPlayer();
}

void Asteroids::updateGame(float delta)
{
	movePlayer(delta);
}

void Asteroids::loadPhysicsEngine()
{
	if (!DLLManager::getInstance().loadDLL("PhysicsEngine.dll"))
	{
		std::cerr << "Failed to load PhysicsEngine.dll" << std::endl;
		return;
	}

	setForwardDirection = DLLManager::getInstance().getFunction<void(*)(GameObject*, glm::vec3)>("PhysicsEngine.dll", "setForwardDirection");
	applyThrust = DLLManager::getInstance().getFunction<void(*)(GameObject*, float)>("PhysicsEngine.dll", "applyThrust");
	updatePhysics = DLLManager::getInstance().getFunction<void(*)(GameObject*, float)>("PhysicsEngine.dll", "updatePhysics");
	checkCollisionRadius = DLLManager::getInstance().getFunction<bool(*)(const GameObject*, const GameObject*, float, float)>("PhysicsEngine.dll", "checkCollisionRadius");
	checkCollisionAABB = DLLManager::getInstance().getFunction<bool(*)(const GameObject*, const GameObject*, const glm::vec3&, const glm::vec3&)>("PhysicsEngine.dll", "checkCollisionAABB");
}

void Asteroids::loadShaders()
{
	ShaderManager::getInstance().loadShader("ADS", "..\\res\\ADS.vert", "..\\res\\ADS.frag");

	// Bind UBO to Shader
	UBOManager::getInstance().bindUBOToShader("Matrices", ShaderManager::getInstance().getShader("ADS")->ID(), "Matrices");
}

void Asteroids::initPlayer()
{
	_playerMesh.loadModel("..\\res\\ship.obj");
	_playerTexture.init("..\\res\\bricks.jpg");

	TransformManager::getInstance().addTransform("player", Transform(glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1)));

	_player = new GameObject(&_playerMesh, &TransformManager::getInstance().getTransform("player"), ShaderManager::getInstance().getShader("ADS").get());
	_gameObjects.push_back(_player);
}

void Asteroids::movePlayer(float delta)
{
	if (!_player) return;

	Transform& playerTransform = *(_player->transform);

	const Uint8* kbState = SDL_GetKeyboardState(NULL);

	// move forward and back by applying positive and negative thrust
	if (kbState[SDL_SCANCODE_W])
	{
		applyThrust(_player, _playerSpeed * delta);
	}

	if (kbState[SDL_SCANCODE_S])
	{
		applyThrust(_player, -_playerSpeed * delta);
	}

	// increase and decrease Y rotation
	// calculate new forward direction from 
	//forward.x = cos(pitch) * sin(yaw);
	//forward.y = -sin(pitch);
	//forward.z = cos(pitch) * cos(yaw);
	if (kbState[SDL_SCANCODE_A])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, playerTransform.rot.y + glm::radians(_playerRotSpeed * delta), 0.0f);
		playerTransform.rot = newRotation;
		setForwardDirection(_player, glm::vec3(
			(cos(newRotation.x) * sin(newRotation.y)),
			-sin(newRotation.x),
			(cos(newRotation.x) * cos(newRotation.y))));
	}
	if (kbState[SDL_SCANCODE_D])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, playerTransform.rot.y - glm::radians(_playerRotSpeed * delta), 0.0f);
		playerTransform.rot = newRotation;
		setForwardDirection(_player, glm::vec3(
			(cos(newRotation.x) * sin(newRotation.y)),
			-sin(newRotation.x),
			(cos(newRotation.x) * cos(newRotation.y))));
	}

	playerTransform.pos += _player->velocity * delta;

	float maxX = 10;
	float maxY = 800;

	if (playerTransform.pos.x > maxX)
	{
		playerTransform.pos.x = -maxX;
	}
	updatePhysics(_player, delta);
}