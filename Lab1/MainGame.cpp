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
	MeshManager::getInstance().loadMesh("PlayerShip", "../res/meshes/ship.obj");
	MeshManager::getInstance().loadMesh("Asteroid", "../res/meshes/asteroid1.obj");
	MeshManager::getInstance().loadMesh("Sphere", "../res/meshes/round.obj");
}

void MainGame::loadShaders()
{
	ShaderManager::getInstance().loadShader("ScreenWrap", true, "../res/shaders/shader.vert", "../res/shaders/shader.frag", "../res/shaders/shader.geom");
	// bind UBOs
	UBOManager::getInstance().bindUBOToShader("Matrices", ShaderManager::getInstance().getShader("ScreenWrap")->ID(), "Matrices");
	UBOManager::getInstance().bindUBOToShader("MatInfo", ShaderManager::getInstance().getShader("ScreenWrap")->ID(), "MatInfo");
	ShaderManager::getInstance().getShader("ScreenWrap")->Bind();
	ShaderManager::getInstance().getShader("ScreenWrap")->setInt("colourSampler", 0);
	ShaderManager::getInstance().getShader("ScreenWrap")->setInt("normalSampler", 1);

	// screen wrapping effect is done in the geometry shader
	// by excluding that, it acts as the same shader without that effect
	ShaderManager::getInstance().loadShader("Default", false, "../res/shaders/shader.vert", "../res/shaders/shader.frag");
	// bind UBOs
	UBOManager::getInstance().bindUBOToShader("Matrices", ShaderManager::getInstance().getShader("Default")->ID(), "Matrices");
	UBOManager::getInstance().bindUBOToShader("MatInfo", ShaderManager::getInstance().getShader("Default")->ID(), "MatInfo");
	ShaderManager::getInstance().getShader("Default")->Bind();
	ShaderManager::getInstance().getShader("Default")->setInt("colourSampler", 0);
	ShaderManager::getInstance().getShader("Default")->setInt("normalSampler", 1);
}

void MainGame::loadTextures()
{
	TextureManager::getInstance().loadTexture("RockColour", "../res/textures/RockColour.png");
	TextureManager::getInstance().loadTexture("RockNormal", "../res/textures/RockNormal.png");
	TextureManager::getInstance().loadTexture("MetalColour", "../res/textures/MetalColour.png");
	TextureManager::getInstance().loadTexture("MetalNormal", "../res/textures/MetalNormal.png");
}

void MainGame::loadSounds()
{
	SoundManager::getInstance().loadSound("Laser", "../res/audio/laser.wav");
	SoundManager::getInstance().loadSound("Thruster", "../res/audio/thruster.wav");
	SoundManager::getInstance().loadSound("Explosion", "../res/audio/explosioncrunch.wav");
}

void MainGame::setupUBOs()
{
	UBOManager::getInstance().createUBO("Matrices", sizeof(glm::mat4) * 3, 0);

	glm::mat4 identity = glm::mat4(1.0f);
	UBOManager::getInstance().updateUBOData("Matrices", 0, glm::value_ptr(identity), sizeof(glm::mat4));
	UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4), glm::value_ptr(_camera->getView()), sizeof(glm::mat4));
	UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4) * 2, glm::value_ptr(_camera->getProjection()), sizeof(glm::mat4));

	const float f = 1.0f;
	UBOManager::getInstance().createUBO("MatInfo", sizeof(float), 1);
	UBOManager::getInstance().updateUBOData("MatInfo", 0, &f, sizeof(float));
}

void MainGame::initSystems()
{
	srand(time(0));
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	_game.initCamera(_gameDisplay.getWidth(), _gameDisplay.getHeight());
	_camera = &_game._camera;
	_renderer.init(&_gameDisplay, _camera);
	setupUBOs();
	loadShaders();
	loadMeshes();
	loadTextures();
	loadSounds();
	_game.initialiseGame();
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