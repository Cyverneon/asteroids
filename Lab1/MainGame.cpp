#include "MainGame.h"
#include "Camera.h"
#include "DLLManager.h"
#include <glm/gtc/type_ptr.hpp>  
#include <iostream>
#include <string>


MainGame::MainGame()
	: _gameDisplay("OpenGL Game", 1980, 1024), // Initialize the display wrapper
	_gameState(GameState::PLAY), counter(0.0f) 
{
	fixedTimeStep = 1.0f / getRefreshRate(); // Dynamically set refresh-based time step
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems() {

	loadMeshes();
	loadTextures();
	setupUBOs();
	loadShaders();
	setupCamera();
	setupTransforms();
	createGameObjects();
	loadPhysicsEngine();
	initPlayer(&playerMesh);
}

void MainGame::initPlayer(Mesh *playerMesh) {
	playerTransform = Transform(glm::vec3(0.0f, 0.0f, 0.0f));
	player = new GameObject(playerMesh, &playerTransform, ShaderManager::getInstance().getShader("ADS").get());
}

float MainGame::getRefreshRate() {
	SDL_DisplayMode mode;
	if (SDL_GetCurrentDisplayMode(0, &mode) == 0 && mode.refresh_rate > 0) {
		float rate = static_cast<float>(mode.refresh_rate);
		std::cout << "[DEBUG] Detected Refresh Rate: " << rate << " Hz" << std::endl;
		return rate;
	}
	std::cout << "[DEBUG] Could not detect refresh rate. Defaulting to 60 Hz." << std::endl;
	return 60.0f; // Fallback if query fails or returns 0
}

void MainGame::calculateDeltaTime() {
	float currentFrameTime = SDL_GetTicks() / 1000.0f; // Get current time in seconds
	deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
}

void MainGame::loadPhysicsEngine() {
	if (!DLLManager::getInstance().loadDLL("PhysicsEngine.dll")) {
		std::cerr << "Failed to load PhysicsEngine.dll" << std::endl;
		return;
	}

	// Retrieve physics functions using the template method
	setForwardDirection = DLLManager::getInstance().getFunction<void(*)(GameObject*, glm::vec3)>("PhysicsEngine.dll", "setForwardDirection");
	applyThrust = DLLManager::getInstance().getFunction<void(*)(GameObject*, float)>("PhysicsEngine.dll", "applyThrust");
	updatePhysics = DLLManager::getInstance().getFunction<void(*)(GameObject*, float)>("PhysicsEngine.dll", "updatePhysics");
	checkCollisionRadius = DLLManager::getInstance().getFunction<bool(*)(const GameObject*, const GameObject*, float, float)>("PhysicsEngine.dll", "checkCollisionRadius");
	checkCollisionAABB = DLLManager::getInstance().getFunction<bool(*)(const GameObject*, const GameObject*, const glm::vec3&, const glm::vec3&)>("PhysicsEngine.dll", "checkCollisionAABB");
}

// 🔹 Loads Meshes
void MainGame::loadMeshes() {
	susanna.loadModel("..\\res\\asteroid1.obj");
	playerMesh.loadModel("..\\res\\monkey3.obj");
}

// 🔹 Loads Textures
void MainGame::loadTextures() {
	texture.init("..\\res\\bricks.jpg");
}

// 🔹 Sets up UBOs for matrix data
void MainGame::setupUBOs() {
	UBOManager::getInstance().createUBO("Matrices", sizeof(glm::mat4) * 3, 0);

	// Initialize with identity matrices to avoid garbage data
	glm::mat4 identity = glm::mat4(1.0f);
	UBOManager::getInstance().updateUBOData("Matrices", 0, glm::value_ptr(identity), sizeof(glm::mat4));
	UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4), glm::value_ptr(identity), sizeof(glm::mat4));
	UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4) * 2, glm::value_ptr(identity), sizeof(glm::mat4));
}

// Loads and sets up shaders
void MainGame::loadShaders() {
	ShaderManager::getInstance().loadShader("ADS", "..\\res\\ADS.vert", "..\\res\\ADS.frag");
	setActiveShader("ADS");

	// Bind UBO to Shader
	UBOManager::getInstance().bindUBOToShader("Matrices", ShaderManager::getInstance().getShader("ADS")->ID(), "Matrices");
}

// Sets up Camera
void MainGame::setupCamera() {
	myCamera.initCamera(glm::vec3(0, 15, 0), 70.0f,
		(float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
}

// Sets up Transforms for objects
void MainGame::setupTransforms() {
	TransformManager::getInstance().addTransform("susanna", Transform(glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1)));
}

void MainGame::createGameObjects() { //this can be improved, we might return to this at some point and make it implicit

	Transform* transform = &TransformManager::getInstance().getTransform("susanna");
	Shader* shader = ShaderManager::getInstance().getShader("ADS").get();

	// make sure transform exists
	// assert will terminate the program (usually with a message quoting the assert statement) 
	// if its argument turns out to be false.
	assert(transform && "TransformManager failed to return a transform!");
	assert(shader && "ShaderManager failed to return a shader!");

	gameObjects.emplace_back(&susanna, transform, shader);
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		std::cout << deltaTime;
		processInput();
		movePlayer();
		updatePlayer();
		drawGame();
		calculateDeltaTime();
		//bool colliding = checkCollisionRadius(player, &gameObjects[0], 0.8f, 0.8f);
		bool colliding = checkCollisionAABB(player, &gameObjects[0], glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f));
		//std::cout << (colliding ? "true!" : "false!") << std::endl;
		if (colliding)
			glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
		else
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
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

void MainGame::movePlayer()
{
	if (!player) return;  // Ensure player exists before applying physics

	Transform& playerTransform = *(player->transform); // Access player's transform

	const Uint8* kbState = SDL_GetKeyboardState(NULL);

	// Move forward (apply thrust)
	if (kbState[SDL_SCANCODE_W])
	{
		if (applyThrust)
		{
			applyThrust(player, playerSpeed*deltaTime);  // Apply forward force
		}
	}

	// Move backward (apply negative thrust)
	if (kbState[SDL_SCANCODE_S])
	{
		if (applyThrust) {
			applyThrust(player, -playerSpeed*deltaTime);
		}
	}

	// Rotate left (increase Y rotation)
	if (kbState[SDL_SCANCODE_A])
	{
		if (setForwardDirection) {
			glm::vec3 newRotation = glm::vec3(0.0f, playerTransform.rot.y + glm::radians(playerRotSpeed*deltaTime), 0.0f);
			playerTransform.rot = newRotation;

			/*		forward.x = cos(pitch) * sin(yaw);
					forward.y = -sin(pitch);
					forward.z = cos(pitch) * cos(yaw);*/
			setForwardDirection(player, glm::vec3((cos(newRotation.x) * sin(newRotation.y)), -sin(newRotation.x), (cos(newRotation.x) * cos(newRotation.y))));
		}
	}

	// Rotate right (decrease Y rotation)
	if (kbState[SDL_SCANCODE_D])
	{
		if (setForwardDirection) {
			glm::vec3 newRotation = glm::vec3(0.0f, playerTransform.rot.y - glm::radians(playerRotSpeed*deltaTime), 0.0f);
			playerTransform.rot = newRotation;
			setForwardDirection(player, glm::vec3((cos(newRotation.x) * sin(newRotation.y)), -sin(newRotation.x), (cos(newRotation.x) * cos(newRotation.y))));
		}
	}
}

void MainGame::updatePlayer() {
	if (!player) return;

	// Apply velocity to position
	playerTransform.pos += player->velocity * deltaTime;

	float maxX = 10;
	float maxY = 800;

	std::cout << playerTransform.pos.x << std::endl;

	if (playerTransform.pos.x > maxX)
	{
		playerTransform.pos.x = -maxX;
	}
	updatePhysics(player, deltaTime);
}


void MainGame::setActiveShader(const std::string& shaderTag) {
	if (ShaderManager::getInstance().getShader(shaderTag)) {
		activeShaderTag = shaderTag;
		std::cout << "Shader switched to: " << shaderTag << std::endl;
	}
	else {
		std::cerr << "Error: Shader not found - " << shaderTag << std::endl;
	}
}

void MainGame::renderGameObjects() { // this can be quickly improved for coursework (for our asteroids)
	Shader* currentShader = nullptr; // Track active shader to avoid redundant binds

	for (auto& obj : gameObjects) {
		if (obj.shader != currentShader) {
			currentShader = obj.shader;
			currentShader->Bind(); // Bind shader only if switching
		}

		// Update UBO for this object's transform
		glm::mat4 model = obj.transform->GetModel();
		glm::mat4 view = myCamera.getView();
		glm::mat4 projection = myCamera.getProjection();

		UBOManager::getInstance().updateUBOData("Matrices", 0, glm::value_ptr(model), sizeof(glm::mat4));
		UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4), glm::value_ptr(view), sizeof(glm::mat4));
		UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4) * 2, glm::value_ptr(projection), sizeof(glm::mat4));

		obj.mesh->draw();

		float angle = SDL_GetTicks() * 0.0001f; // Convert milliseconds to seconds
		glUniform1f(glGetUniformLocation(currentShader->ID(), "angle"), angle);
	}
}

void MainGame::renderPlayer() {
	if (!player) {
		std::cerr << "Warning: Player pointer is null! Skipping render." << std::endl;
		return;
	}

	// Use the shader that was assigned to the player
	Shader* playerShader = player->shader;
	if (!playerShader) {
		std::cerr << "Error: Player has no assigned shader!" << std::endl;
		return;
	}

	playerShader->Bind();

	glm::mat4 playerModel = player->transform->GetModel();
	glm::mat4 view = myCamera.getView();
	glm::mat4 projection = myCamera.getProjection();

	UBOManager::getInstance().updateUBOData("Matrices", 0, glm::value_ptr(playerModel), sizeof(glm::mat4));
	UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4), glm::value_ptr(view), sizeof(glm::mat4));
	UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4) * 2, glm::value_ptr(projection), sizeof(glm::mat4));

	player->mesh->draw();
}


void MainGame::clearScreenBuffer()
{
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set clear color to blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}

void MainGame::drawGame() {
	clearScreenBuffer();
	renderPlayer();
	renderGameObjects();
	_gameDisplay.swapBuffers();
}

