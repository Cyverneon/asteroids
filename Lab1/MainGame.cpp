#include "MainGame.h"

MainGame::MainGame()
	: _gameDisplay("OpenGL Game", 1920, 1080), // Initialize the display wrapper
	_gameState(GameState::PLAY)
{
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::loadMeshes()
{
	MeshManager::getInstance().loadMesh("PlayerShip", "../res/asteroid1.obj");
}

void MainGame::loadShaders()
{
	ShaderManager::getInstance().loadShader("DefaultShader", "../res/shaders/shader.vert", "../res/shaders/shader.frag");

	UBOManager::getInstance().bindUBOToShader("Matrices", ShaderManager::getInstance().getShader("DefaultShader")->ID(), "Matrices");
}

void MainGame::loadTextures()
{
	TextureManager::getInstance().loadTexture("RockColour", "../res/textures/RockColour.png");
	TextureManager::getInstance().loadTexture("RockNormal", "../res/textures/RockNormal.png");
}

void MainGame::initSystems()
{
	_renderer.setupUBOs();
	_renderer.init(&_gameDisplay, &_camera);
	loadShaders();
	loadMeshes();
	loadTextures();
	_game.initialiseGame();
	setupCamera();

	glm::vec3 test = glm::unProject(glm::vec3(0, 0, 1), glm::mat4(1.0f), _camera.getProjection(), glm::vec4(0, 0, _gameDisplay.getWidth(), _gameDisplay.getHeight()));
	std::cout << test.x << ", " << test.y << ", " << test.z << std::endl;

	test = glm::unProject(glm::vec3(1920, 1080, 1), glm::mat4(1.0f), _camera.getProjection(), glm::vec4(0, 0, _gameDisplay.getWidth(), _gameDisplay.getHeight()));
	std::cout << test.x << ", " << test.y << ", " << test.z << std::endl;
}

void MainGame::calculateDeltaTime()
{
	float currentFrameTime = SDL_GetTicks() / 1000.0f; // divide by 1000 to convert from milliseconds to seconds
	_deltaTime = currentFrameTime - _lastFrameTime;
	_lastFrameTime = currentFrameTime;
}

void MainGame::setupCamera()
{
	_camera.initCamera(glm::vec3(0, 15, 0), 70.0f,
		(float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		_game.updateGame(_deltaTime);
		_renderer.drawGame();
		calculateDeltaTime();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) // Get and process events
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		}
	}
}