#pragma once
#include <iostream>
#include <vector>
#include <windows.h>  // Required for LoadLibrary & GetProcAddress
#include "Renderer.h"
#include "GameLogic.h"
#include "Camera.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void loadMeshes();
	void loadShaders();
	void loadTextures();
	void initSystems();
	void processInput();
	void gameLoop();
	void calculateDeltaTime();
	void setupCamera();

	DisplayFacade _gameDisplay;
	GameState _gameState;
	Camera _camera;

	float _deltaTime = 0.0f;
	float _lastFrameTime = 0.0f;

	GameLogic _game;
	Renderer _renderer;
};

