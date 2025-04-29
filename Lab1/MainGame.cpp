#include "MainGame.h"

MainGame::MainGame()
	: _gameDisplay("OpenGL Game", 1920, 1080), // Initialize the display wrapper
	_gameState(GameState::PLAY)
{
	_fixedTimeStep = 1.0f / getRefreshRate(); // Dynamically set refresh-based time step
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_renderer.setupUBOs();
	_renderer.init(&_gameDisplay, &_camera);
	_game.loadShaders();
	_game.loadMeshes();
	_game.loadTextures();
	_game.initialiseGame();
	setupCamera();

	glm::vec3 test = glm::unProject(glm::vec3(0, 0, 1), glm::mat4(1.0f), _camera.getProjection(), glm::vec4(0, 0, _gameDisplay.getWidth(), _gameDisplay.getHeight()));
	std::cout << test.x << ", " << test.y << ", " << test.z << std::endl;

	test = glm::unProject(glm::vec3(1920, 1080, 1), glm::mat4(1.0f), _camera.getProjection(), glm::vec4(0, 0, _gameDisplay.getWidth(), _gameDisplay.getHeight()));
	std::cout << test.x << ", " << test.y << ", " << test.z << std::endl;
}

float MainGame::getRefreshRate()
{
	SDL_DisplayMode mode;
	if (SDL_GetCurrentDisplayMode(0, &mode) == 0 && mode.refresh_rate > 0)
	{
		float rate = static_cast<float>(mode.refresh_rate);
		std::cout << "[DEBUG] Detected Refresh Rate: " << rate << " Hz" << std::endl;
		return rate;
	}
	std::cout << "[DEBUG] Could not detect refresh rate. Defaulting to 60 Hz." << std::endl;
	return 60.0f; // Fallback if query fails or returns 0
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