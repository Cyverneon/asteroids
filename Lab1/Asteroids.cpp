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

void Asteroids::loadMeshes()
{
	MeshManager::getInstance().loadMesh("PlayerShip", "..\\res\\ship.obj");
}

void Asteroids::loadShaders()
{
	ShaderManager::getInstance().loadShader("ADS", "..\\res\\ADS.vert", "..\\res\\ADS.frag");

	// Bind UBO to Shader
	UBOManager::getInstance().bindUBOToShader("Matrices", ShaderManager::getInstance().getShader("ADS")->ID(), "Matrices");
}

void Asteroids::loadTextures()
{
	TextureManager::getInstance().loadTexture("Water", "..\\res\\water.jpg");
	TextureManager::getInstance().loadTexture("Bricks", "..\\res\\bricks.jpg");
}

void Asteroids::initPlayer()
{
	TransformManager::getInstance().addTransform("player", Transform(glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1)));

	_player = GameObjectManager::getInstance().createGameObject(
		"Player",
		&TransformManager::getInstance().getTransform("player"),
		"PlayerShip",
		"ADS");
}

void Asteroids::movePlayer(float delta)
{
	if (!_player) return;

	Transform& playerTransform = *(_player->_transform);

	const Uint8* kbState = SDL_GetKeyboardState(NULL);
	
	if (kbState[SDL_SCANCODE_W])
	{
		applyThrust(_player.get(), _playerSpeed * delta);
	}

	 //backwards movement was disabled because it doesn't really make sense for the ship's thrusters to be able to reverse
	 //and isn't possible in the actual asteroids game
	 //left it here so it can be re-enabled if wanted
	if (kbState[SDL_SCANCODE_S])
	{
		applyThrust(_player.get(), -_playerSpeed * delta);
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
		setForwardDirection(_player.get(), glm::vec3(
			(cos(newRotation.x) * sin(newRotation.y)),
			-sin(newRotation.x),
			(cos(newRotation.x) * cos(newRotation.y))));
	}
	if (kbState[SDL_SCANCODE_D])
	{
		glm::vec3 newRotation = glm::vec3(0.0f, playerTransform.rot.y - glm::radians(_playerRotSpeed * delta), 0.0f);
		playerTransform.rot = newRotation;
		setForwardDirection(_player.get(), glm::vec3(
			(cos(newRotation.x) * sin(newRotation.y)),
			-sin(newRotation.x),
			(cos(newRotation.x) * cos(newRotation.y))));
	}

	playerTransform.pos += _player->velocity * delta;

	//float minX = -12.0;
	//float maxX = 12.0;
	//float minY = -7.0;
	//float maxY = 7.0;

	//if (playerTransform.pos.x > maxX)
	//{
	//	playerTransform.pos.x = minX;
	//}
	//else if (playerTransform.pos.x < minX)
	//{
	//	playerTransform.pos.x = maxX;
	//}
	//if (playerTransform.pos.z > maxY)
	//{
	//	playerTransform.pos.z = minY;
	//}
	//else if (playerTransform.pos.z < minY)
	//{
	//	playerTransform.pos.z = maxY;
	//}
	updatePhysics(_player.get(), delta);
}