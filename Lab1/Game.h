#pragma once

#include <vector>
#include "GameObject.h"
#include "DLLManager.h"
#include "GameObjectManager.h"
#include "TransformManager.h"
#include "ShaderManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "UBOManager.h"

class Game
{
public:
	Game() {};
	~Game() {};

	virtual void loadMeshes() {};
	virtual void loadShaders() {};
	virtual void loadTextures() {};
	virtual void initialiseGame() {};
	virtual void updateGame(float delta) {};
};

