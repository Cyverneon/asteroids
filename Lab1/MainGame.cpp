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
	ShaderManager::getInstance().loadShader("ScreenWrap", true, "../res/shaders/shader.vert", "../res/shaders/shader.frag", "../res/shaders/shader.geom");
	UBOManager::getInstance().bindUBOToShader("Matrices", ShaderManager::getInstance().getShader("ScreenWrap")->ID(), "Matrices");
	ShaderManager::getInstance().getShader("ScreenWrap")->Bind();
	ShaderManager::getInstance().getShader("ScreenWrap")->setInt("colourSampler", 0);
	ShaderManager::getInstance().getShader("ScreenWrap")->setInt("normalSampler", 1);

	// screen wrapping effect is done in the geometry shader
	// so by excluding that, it acts as the same shader without that effect
	ShaderManager::getInstance().loadShader("Default", false, "../res/shaders/shader.vert", "../res/shaders/shader.frag");
	UBOManager::getInstance().bindUBOToShader("Matrices", ShaderManager::getInstance().getShader("Default")->ID(), "Matrices");
	ShaderManager::getInstance().getShader("Default")->Bind();
	ShaderManager::getInstance().getShader("Default")->setInt("colourSampler", 0);
	ShaderManager::getInstance().getShader("Default")->setInt("normalSampler", 1);
}

void MainGame::loadTextures()
{
	TextureManager::getInstance().loadTexture("RockColour", "../res/textures/RockColour.png");
	TextureManager::getInstance().loadTexture("RockNormal", "../res/textures/RockNormal.png");
}

void MainGame::initSystems()
{
	_renderer.setupUBOs();
	_renderer.init(&_gameDisplay, &_game._camera);
	loadShaders();
	loadMeshes();
	loadTextures();
	_game.initialiseGame();
	_game.initCamera(_gameDisplay.getWidth(), _gameDisplay.getHeight());
}

void MainGame::calculateDeltaTime()
{
	float currentFrameTime = SDL_GetTicks() / 1000.0f; // divide by 1000 to convert from milliseconds to seconds
	_deltaTime = currentFrameTime - _lastFrameTime;
	_lastFrameTime = currentFrameTime;
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