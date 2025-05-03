#include "GameLogic.h"

GameLogic::GameLogic()
{

}

GameLogic::~GameLogic()
{

}

void GameLogic::initialiseGame()
{
	loadPhysicsEngine();
	initPlayer();
}

void GameLogic::updateGame(float delta)
{
	movePlayer(delta);
}

void GameLogic::loadPhysicsEngine()
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

void GameLogic::initCamera(float width, float height)
{
	_camera.initCamera(glm::vec3(0, 15, 0), 70.0f, width / height, 0.01f, 1000.0f);
}

void GameLogic::initPlayer()
{
	_player = GameObjectManager::getInstance().createGameObject(
		"Player",
		"PlayerShip",
		"DefaultShader",
		std::vector<std::string>{"RockColour", "RockNormal"},
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1)
	);

	_playerTransform = _player->_transform;
}

void GameLogic::createAsteroids()
{

}

void GameLogic::movePlayer(float delta)
{
	if (!_player) return;

	const Uint8* kbState = SDL_GetKeyboardState(NULL);

	if (kbState[SDL_SCANCODE_W])
	{
		applyThrust(_player.get(), _playerSpeed * delta);
	}

	// backwards movement was disabled because it doesn't really make sense for the ship's thrusters to be able to reverse
	// and isn't possible in actual asteroids
	// left it here so it can be re-enabled if wanted
	//if (kbState[SDL_SCANCODE_S])
	//{
	//	applyThrust(_player.get(), -_playerSpeed * delta);
	//}

	// increase and decrease Y rotation
	// calculate new forward direction from 
	//forward.x = cos(pitch) * sin(yaw);
	//forward.y = -sin(pitch);
	//forward.z = cos(pitch) * cos(yaw);
	if (kbState[SDL_SCANCODE_A])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, _playerTransform->rot.y + glm::radians(_playerRotSpeed * delta), 0.0f);
		_playerTransform->rot = newRotation;
		setForwardDirection(_player.get(), glm::vec3(
			(cos(newRotation.x) * sin(newRotation.y)),
			-sin(newRotation.x),
			(cos(newRotation.x) * cos(newRotation.y))));
	}
	if (kbState[SDL_SCANCODE_D])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, _playerTransform->rot.y - glm::radians(_playerRotSpeed * delta), 0.0f);
		_playerTransform->rot = newRotation;
		setForwardDirection(_player.get(), glm::vec3(
			(cos(newRotation.x) * sin(newRotation.y)),
			-sin(newRotation.x),
			(cos(newRotation.x) * cos(newRotation.y))));
	}

	_playerTransform->pos += _player->velocity * delta;

	float minX = -12.0;
	float maxX = 12.0;
	float minY = -7.0;
	float maxY = 7.0;

	if (_playerTransform->pos.x > maxX)
	{
		_playerTransform->pos.x = minX;
	}
	else if (_playerTransform->pos.x < minX)
	{
		_playerTransform->pos.x = maxX;
	}
	if (_playerTransform->pos.z > maxY)
	{
		_playerTransform->pos.z = minY;
	}
	else if (_playerTransform->pos.z < minY)
	{
		_playerTransform->pos.z = maxY;
	}
	updatePhysics(_player.get(), delta);
}