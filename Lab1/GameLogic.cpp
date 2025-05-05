#include "GameLogic.h"

GameLogic::GameLogic() {}

GameLogic::~GameLogic() {}

void GameLogic::initialiseGame()
{
	initPlayer();
	spawnAsteroidRound();
	ShaderManager::getInstance().getShader("ScreenWrap")->Bind();
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("minX", -_maxX);
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("maxX", _maxX);
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("minZ", -_maxZ);
	ShaderManager::getInstance().getShader("ScreenWrap")->setFloat("maxZ", _maxZ);
}

void GameLogic::updateGame(float delta)
{
	processInput(delta);
	movePlayer(delta);
	updateAsteroids(delta);
	updateBullets(delta);
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
		glm::vec3(0, glm::radians(180.0), 0),
		glm::vec3(0.5, 0.5, 0.5)
	);

	_player->_physicsObject._collisionRadius = 0.3;
}

void GameLogic::spawnBullet()
{
	std::string bulletTag = "PlayerBullet" + std::to_string(rand());

	std::shared_ptr<GameObject> bullet = GameObjectManager::getInstance().createGameObject(
		bulletTag,
		"Sphere",
		"Default",
		std::vector<std::string>{"MetalColour", "MetalNormal"},
		_player->_transform->pos,
		_player->_transform->rot,
		glm::vec3(0.05)
	);

	if (bullet == nullptr)
		return;

	bullet->_physicsObject._collisionRadius = 0.05;

	_playerBullets.push_back(bulletTag);
}

void GameLogic::spawnAsteroid(std::string size, glm::vec3 pos, glm::vec3 rot)
{
	std::string asteroidTag = "Asteroid" + size + std::to_string(rand());

	std::shared_ptr<GameObject> asteroid = GameObjectManager::getInstance().createGameObject(
		asteroidTag,
		"Asteroid",
		"Default",
		std::vector<std::string>{"RockColour", "RockNormal"},
		pos,
		rot,
		glm::vec3(_asteroidSizes.at(size), _asteroidSizes.at(size), _asteroidSizes.at(size))
	);

	if (asteroid == nullptr)
		return;

	asteroid->_physicsObject._collisionRadius = _asteroidSizes.at(size);

	_asteroids.push_back(asteroidTag);
}

void GameLogic::spawnAsteroidRound()
{
	for (int i = 0; i < 3; i++)
	{
		// asteroids only spawn on the outside of the screen so the player has time to react to them
		// this is done by getting random value within half the max coordinates ( offset of the top and right edges from the center) then adding half again
		// so asteroids spawn in topmost and rightmost quarter
		// then 50/50 chance of becoming negative so they can also spawn in leftmost and bottommost quarters
		float xPos = ((float)rand()/RAND_MAX * (_maxX / 2)) + _maxX / 2;
		xPos = (rand() % 2 == 0) ? xPos : -xPos;

		float zPos = ((float)rand()/RAND_MAX * (_maxZ / 2)) + _maxZ / 2;
		zPos = (rand() % 2 == 0) ? zPos : -zPos;

		// rotate asteroid to be pointing towards the middle of the screen where player spawns
		// with some random offset between -0.5 and 0.5 (radians) to feel more natural
		float yRot = atan2(-xPos, -zPos) + (float)rand()/RAND_MAX - 0.5;

		spawnAsteroid("large", glm::vec3(xPos, 0, zPos), glm::vec3(0, yRot, 0));
	}
}

void GameLogic::processInput(float delta)
{
	_kbState = SDL_GetKeyboardState(NULL);

	if (_kbState[SDL_SCANCODE_W])
		_player->_physicsObject.applyThrust(_playerSpeed, delta);

	// backwards movement was disabled because it doesn't really make sense for the ship's thrusters to be able to reverse
	// and isn't possible in actual asteroids
	// left it here so it can be re-enabled if wanted
	//if (_kbState[SDL_SCANCODE_S])
	//	_player->_physicsObject.applyThrust(-_playerSpeed, delta);

	// increase and decrease Y rotation
	if (_kbState[SDL_SCANCODE_A])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, _player->_transform->rot.y + glm::radians(_playerRotSpeed * delta), 0.0f);
		_player->_physicsObject.updateRot(newRotation);
	}
	if (_kbState[SDL_SCANCODE_D])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, _player->_transform->rot.y - glm::radians(_playerRotSpeed * delta), 0.0f);
		_player->_physicsObject.updateRot(newRotation);
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
	_player->_physicsObject.moveByVel(delta);
	_player->_physicsObject.wrapPosition(glm::vec2(_maxX, _maxZ), glm::vec2(0.0));
	_player->_physicsObject.updatePhysics(delta);
}

void GameLogic::updateAsteroids(float delta)
{
	for (int i = 0; i < _asteroids.size(); i++)
	{
		// move
		std::shared_ptr<GameObject> asteroid = GameObjectManager::getInstance().getGameObject(_asteroids[i]);
		asteroid->_physicsObject.moveByAmount(_asteroidSpeed, delta);
		// add some offset to the position wrap, so the asteroids go fully offscreen
		// previously asteroids would snap round as the center hit the edge of the screen and use the screen wrap shader like the player
		// but this felt too visually busy with a lot of asteroids on screen
		asteroid->_physicsObject.wrapPosition(glm::vec2(_maxX, _maxZ), glm::vec2(1.5));

		// check bullet collisions
		for (int j = 0; j < _playerBullets.size(); j++)
		{
			if (asteroid->_physicsObject.checkCollision(&GameObjectManager::getInstance().getGameObject(_playerBullets[j])->_physicsObject))
			{
				// smallest asteroids should not spawn any sub asteroids on destruction
				if (_asteroids[i].rfind("Asteroidsmall", 0) != 0)
				{
					std::string size = (_asteroids[i].rfind("Asteroidlarge", 0) == 0) ? "medium" : "small";
					// get y rotation of bullet to spawn two asteroids at 90 and -90 degree offset of direction asteroid was hit by
					float yRot = GameObjectManager::getInstance().getGameObject(_playerBullets[j])->_transform->rot.y;
					// spawn sub-asteroids
					spawnAsteroid(size, asteroid->_transform->pos, glm::vec3(asteroid->_transform->rot.x, yRot + glm::radians(90.0), asteroid->_transform->rot.z));
					spawnAsteroid(size, asteroid->_transform->pos, glm::vec3(asteroid->_transform->rot.x, yRot + glm::radians(-90.0), asteroid->_transform->rot.z));
				}
				// destroy asteroid
				GameObjectManager::getInstance().removeGameObject(_asteroids[i]);
				_asteroids.erase(_asteroids.begin() + i);
				i--;
				// destroy bullet
				GameObjectManager::getInstance().removeGameObject(_playerBullets[j]);
				_playerBullets.erase(_playerBullets.begin() + j);
				break;
			}
		}
	}
}

void GameLogic::updateBullets(float delta)
{
	for (int i = 0; i < _playerBullets.size(); i++)
	{
		std::shared_ptr<GameObject> bullet = GameObjectManager::getInstance().getGameObject(_playerBullets[i]);
		bullet->_physicsObject.moveByAmount(_bulletSpeed, delta);
		if ((bullet->_transform->pos.x > _maxX) ||
			(bullet->_transform->pos.x < -_maxX) ||
			(bullet->_transform->pos.z > _maxZ) ||
			(bullet->_transform->pos.z < -_maxZ))
		{
			GameObjectManager::getInstance().removeGameObject(_playerBullets[i]);
			_playerBullets.erase(_playerBullets.begin() + i);
			i--;
		}
	}
}

void GameLogic::checkPlayerDeath()
{

}