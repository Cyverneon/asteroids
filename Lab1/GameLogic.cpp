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
	spawnAsteroidRound();
	ShaderManager::getInstance().getShader("ScreenWrap")->Bind();
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("minX", -maxX);
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("maxX", maxX);
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("minZ", -maxZ);
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("maxZ", maxZ);
}

void GameLogic::updateGame(float delta)
{
	processInput(delta);
	movePlayer(delta);
	moveAsteroids(delta);
	moveBullets(delta);
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
}

void GameLogic::spawnBullet()
{
	std::string bulletTag = "PlayerBullet" + std::to_string(rand());

	std::shared_ptr<GameObject> bullet = GameObjectManager::getInstance().createGameObject(
		bulletTag,
		"Bullet",
		"Default",
		std::vector<std::string>{"MetalColour", "MetalNormal"},
		_player->_transform->pos,
		_player->_transform->rot,
		glm::vec3(1.0, 1.0, 1.0)
	);

	setForwardDirectionFromRot(bullet.get(), bullet->_transform->rot);

	_playerBullets.push_back(bulletTag);
}

void GameLogic::spawnAsteroid(std::string tag, glm::vec3 pos, glm::vec3 rot)
{
	std::shared_ptr<GameObject> asteroid = GameObjectManager::getInstance().createGameObject(
		tag + std::to_string(time(0)),
		"Asteroid",
		"ScreenWrap",
		std::vector<std::string>{"RockColour", "RockNormal"},
		pos,
		rot,
		glm::vec3(1.5, 1.5, 1.5)
	);

	setForwardDirectionFromRot(asteroid.get(), asteroid->_transform->rot);

	_asteroids.push_back(asteroid);
}

void GameLogic::spawnAsteroidRound()
{
	for (int i = 0; i < 3; i++)
	{
		// asteroids only spawn on the outside of the screen so the player has time to react to them
		// this is done by getting random value within half the max coordinates ( offset of the top and right edges from the center) then adding half again
		// so asteroids spawn in topmost and rightmost quarter
		// then 50/50 chance of becoming negative so they can also spawn in leftmost and bottommost quarters
		float xPos = ((float)rand()/RAND_MAX * (maxX / 2)) + maxX / 2;
		xPos = (rand() % 2 == 0) ? xPos : -xPos;

		float zPos = ((float)rand()/RAND_MAX * (maxZ / 2)) + maxZ / 2;
		zPos = (rand() % 2 == 0) ? zPos : -zPos;

		// rotate asteroid to be pointing towards the middle of the screen where player spawns
		// with some random offset between -0.5 and 0.5 (radians) to feel more natural
		float yRot = atan2(-xPos, -zPos) + (float)rand()/RAND_MAX - 0.5;

		spawnAsteroid("AsteroidBig" + std::to_string(i), glm::vec3(xPos, 0, zPos), glm::vec3(0, yRot, 0));
	}
}

glm::vec3 GameLogic::wrapObjectPosition(glm::vec3 pos)
{
	if (pos.x > maxX)
		pos.x = -maxX;
	else if (pos.x < -maxX)
		pos.x = maxX;
	if (pos.z > maxZ)
		pos.z = -maxZ;
	else if (pos.z < -maxZ)
		pos.z = maxZ;
	return pos;
}

void GameLogic::processInput(float delta)
{
	_kbState = SDL_GetKeyboardState(NULL);

	if (_kbState[SDL_SCANCODE_W])
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
	if (_kbState[SDL_SCANCODE_A])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, _player->_transform->rot.y + glm::radians(_playerRotSpeed * delta), 0.0f);
		_player->_transform->rot = newRotation;
		setForwardDirectionFromRot(_player.get(), newRotation);
	}
	if (_kbState[SDL_SCANCODE_D])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, _player->_transform->rot.y - glm::radians(_playerRotSpeed * delta), 0.0f);
		_player->_transform->rot = newRotation;
		setForwardDirectionFromRot(_player.get(), newRotation);
	}

	if (_kbState[SDL_SCANCODE_SPACE])
	{
		if (!_bulletFired)
		{
			spawnBullet();
			_bulletFired = true;
		}
	}
	else
	{
		_bulletFired = false;
	}
}

void GameLogic::movePlayer(float delta)
{
	_player->_transform->pos += _player->velocity * delta;

	_player->_transform->pos = wrapObjectPosition(_player->_transform->pos);

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

void GameLogic::moveBullets(float delta)
{
	for (int i = 0; i < _playerBullets.size(); i++)
	{
		std::shared_ptr<GameObject> bullet = GameObjectManager::getInstance().getGameObject(_playerBullets[i]);
		bullet->_transform->pos += glm::normalize(bullet->forwardDirection) * _bulletSpeed * delta;
		if ((bullet->_transform->pos.x > maxX) ||
			(bullet->_transform->pos.x < -maxX) ||
			(bullet->_transform->pos.z > maxZ) ||
			(bullet->_transform->pos.z < -maxZ))
		{
			GameObjectManager::getInstance().removeGameObject(_playerBullets[i]);
			_playerBullets.erase(_playerBullets.begin() + i);
			i--;
		}
	}
}