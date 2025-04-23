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
	void loadMeshes();
	void loadTextures();
	void setupUBOs();
	void loadShaders();
	void setupCamera();
	void renderGameObjects();
	void renderPlayer();
	void setupTransforms();
	void createGameObjects();
	void loadPhysicsEngine();
	void movePlayer();
	void initPlayer(Mesh* playerMesh);
	void updatePlayer(float deltaTime);
	float getRefreshRate();


	DisplayFacade _gameDisplay;
	GameState _gameState;
	Mesh susanna;
	Mesh playerMesh;
	Camera myCamera;
	Texture texture; 
	Transform transform;
	Transform playerTransform;

	float counter;

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;
	float accumulator = 0.0f;
	mutable float fixedTimeStep; // 60 physics updates per second

	GameObject* player; // Store a pointer to the player object
	std::vector<GameObject> gameObjects; // Store all game objects (for future automation)
	std::string activeShaderTag; // Track the active shader

	// Function pointers for physics engine functions
	void (*setForwardDirection)(GameObject*, glm::vec3) = nullptr;
	void (*applyThrust)(GameObject*, float) = nullptr;
	void (*updatePhysics)(GameObject*, float) = nullptr;
	bool (*checkCollisionRadius)(const GameObject*, const GameObject*, float, float) = nullptr;
	bool (*checkCollisionAABB)(const GameObject*, const GameObject*, const glm::vec3&, const glm::vec3&);

	float playerSpeed = 0.5f;
	float playerRotSpeed = 3.0f;
};

