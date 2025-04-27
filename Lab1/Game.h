#pragma once

#include <vector>
#include "GameObject.h"
#include "DLLManager.h"
#include "TransformManager.h"
#include "ShaderManager.h"
#include "UBOManager.h"

class Game
{
public:
	Game() {};
	~Game() {};

	virtual void loadShaders() {};
	virtual void initialiseGame() {};
	virtual void updateGame(float delta) {};

	std::vector<GameObject*> _gameObjects;
};

