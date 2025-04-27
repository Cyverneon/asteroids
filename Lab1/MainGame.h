#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <vector>
#include <windows.h>  // Required for LoadLibrary & GetProcAddress
#include "ShaderManager.h"
#include "TransformManager.h"
#include "UBOManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "DisplayFacade.h" 
#include "GameObject.h"
#include "Game.h"
#include "Asteroids.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void setActiveShader(const std::string& shaderTag);
	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void clearScreenBuffer();
	void calculateDeltaTime();
	void setupUBOs();
	void setupCamera();
	void renderGameObjects();
	float getRefreshRate();


	DisplayFacade _gameDisplay;
	GameState _gameState;
	Camera _camera;

	float _deltaTime = 0.0f;
	float _lastFrameTime = 0.0f;
	float _fixedTimeStep = 0.0f;

	Asteroids _game;
	std::string _activeShaderTag;
};

