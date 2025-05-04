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
	createAsteroids();
	ShaderManager::getInstance().getShader("ScreenWrap")->Bind();
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("minX", minX);
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("maxX", maxX);
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("minZ", minZ);
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("maxZ", maxZ);
}

void GameLogic::updateGame(float delta)
{
	movePlayer(delta);
	moveAsteroids(delta);
}

void GameLogic::loadPhysicsEngine()
{
	if (!DLLManager::getInstance().loadDLL("PhysicsEngine.dll"))
	{
		std::cerr << "Failed to load PhysicsEngine.dll" << std::endl;
		return;
	}

	setForwardDirection = DLLManager::getInstance().getFunction<void(*)(GameObject*, glm::vec3)>("PhysicsEngine.dll", "setForwardDirection");
	setForwardDirectionFromRot = DLLManager::getInstance().getFunction<void(*)(GameObject*, glm::vec3)>("PhysicsEngine.dll", "setForwardDirectionFromRot");
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
		"ScreenWrap",
		std::vector<std::string>{"MetalColour", "MetalNormal"},
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0.5, 0.5, 0.5)
	);

	_playerTransform = _player->_transform;
}

void GameLogic::createAsteroids()
{
	std::vector<float> asteroidRotations = { 35, 160, 230, 10, 305 };
	std::vector<std::pair<float, float>> asteroidPositions = {
		{5, 5 },
		{-10, 1},
		{8, 4},
		{-3, -5},
		{1, -1}
	};

	for (int i = 0; i < 3; i++)
	{
		_asteroids.push_back(GameObjectManager::getInstance().createGameObject(
			"Asteroid" + std::to_string(i),
			"Asteroid",
			"ScreenWrap",
			std::vector<std::string>{"RockColour", "RockNormal"},
			glm::vec3(asteroidPositions[i].first, 0, asteroidPositions[i].second),
			glm::vec3(0, asteroidRotations[i], 0),
			glm::vec3(1.5, 1.5, 1.5)
		));

		setForwardDirectionFromRot(_asteroids[i].get(), _asteroids[i]->_transform->rot);
	}
}

glm::vec3 GameLogic::wrapObjectPosition(glm::vec3 pos)
{
	if (pos.x > maxX)
		pos.x = minX;
	else if (pos.x < minX)
		pos.x = maxX;
	if (pos.z > maxZ)
		pos.z = minZ;
	else if (pos.z < minZ)
		pos.z = maxZ;
	return pos;
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
	if (kbState[SDL_SCANCODE_S])
	{
		applyThrust(_player.get(), -_playerSpeed * delta);
	}

	// increase and decrease Y rotation
	if (kbState[SDL_SCANCODE_A])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, _playerTransform->rot.y + glm::radians(_playerRotSpeed * delta), 0.0f);
		_playerTransform->rot = newRotation;
		setForwardDirectionFromRot(_player.get(), newRotation);
	}
	if (kbState[SDL_SCANCODE_D])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, _playerTransform->rot.y - glm::radians(_playerRotSpeed * delta), 0.0f);
		_playerTransform->rot = newRotation;
		setForwardDirectionFromRot(_player.get(), newRotation);
	}

	_playerTransform->pos += _player->velocity * delta;

	_playerTransform->pos = wrapObjectPosition(_playerTransform->pos);

	updatePhysics(_player.get(), delta);
}

void GameLogic::moveAsteroids(float delta)
{
	for (auto& asteroid : _asteroids)
	{
		asteroid->_transform->pos += glm::normalize(asteroid->forwardDirection) * _asteroidSpeed * delta;
		asteroid->_transform->pos = wrapObjectPosition(asteroid->_transform->pos);
	}
}