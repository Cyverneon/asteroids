#include "MainGame.h"
#include "Camera.h"
#include "DLLManager.h"
#include <glm/gtc/type_ptr.hpp>  
#include <iostream>
#include <string>


MainGame::MainGame()
	: _gameDisplay("OpenGL Game", 1980, 1024), // Initialize the display wrapper
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
	setupUBOs();
	_game.loadShaders();
	_game.initialiseGame();
	setupCamera();
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

void MainGame::setupUBOs()
{
	UBOManager::getInstance().createUBO("Matrices", sizeof(glm::mat4) * 3, 0);

	// Initialize with identity matrices to avoid garbage data
	glm::mat4 identity = glm::mat4(1.0f);
	UBOManager::getInstance().updateUBOData("Matrices", 0, glm::value_ptr(identity), sizeof(glm::mat4));
	UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4), glm::value_ptr(identity), sizeof(glm::mat4));
	UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4) * 2, glm::value_ptr(identity), sizeof(glm::mat4));
}

// Sets up Camera
void MainGame::setupCamera()
{
	_camera.initCamera(glm::vec3(0, 15, 0), 70.0f,
		(float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
}


void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT)
	{
		processInput();
		_game.updateGame(_deltaTime);
		drawGame();
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


void MainGame::setActiveShader(const std::string& shaderTag)
{
	if (ShaderManager::getInstance().getShader(shaderTag))
	{
		_activeShaderTag = shaderTag;
		std::cout << "Shader switched to: " << shaderTag << std::endl;
	}
	else
	{
		std::cerr << "Error: Shader not found - " << shaderTag << std::endl;
	}
}

void MainGame::renderGameObjects()
{
	// track current shader to avoid redundant binds
	Shader* currentShader = nullptr;

	for (auto& obj : _game._gameObjects)
	{
		if (obj->shader != currentShader)
		{
			currentShader = obj->shader;
			currentShader->Bind();
		}

		// Update UBO for this object's transform
		glm::mat4 model = obj->transform->GetModel();
		glm::mat4 view = _camera.getView();
		glm::mat4 projection = _camera.getProjection();

		UBOManager::getInstance().updateUBOData("Matrices", 0, glm::value_ptr(model), sizeof(glm::mat4));
		UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4), glm::value_ptr(view), sizeof(glm::mat4));
		UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4) * 2, glm::value_ptr(projection), sizeof(glm::mat4));

		obj->mesh->draw();

		float angle = SDL_GetTicks() * 0.0001f; // Convert milliseconds to seconds
		glUniform1f(glGetUniformLocation(currentShader->ID(), "angle"), angle);
	}
}

void MainGame::clearScreenBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainGame::drawGame() {
	clearScreenBuffer();
	renderGameObjects();
	_gameDisplay.swapBuffers();
}

