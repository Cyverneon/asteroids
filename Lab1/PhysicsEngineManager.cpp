#include "PhysicsEngineManager.h"

PhysicsEngineManager& PhysicsEngineManager::getInstance()
{
    static PhysicsEngineManager instance;
    return instance;
}

PhysicsEngineManager::PhysicsEngineManager()
{
	if (!DLLManager::getInstance().loadDLL("PhysicsEngine.dll"))
	{
		std::cerr << "Failed to load PhysicsEngine.dll" << std::endl;
		return;
	}

	setForwardDirection = DLLManager::getInstance().getFunction<void(*)(glm::vec3*, const glm::vec3)>("PhysicsEngine.dll", "setForwardDirection");
	setForwardDirectionFromRot = DLLManager::getInstance().getFunction<void(*)(glm::vec3*, const glm::vec3)>("PhysicsEngine.dll", "setForwardDirectionFromRot");
	applyThrust = DLLManager::getInstance().getFunction<void(*)(glm::vec3*, const glm::vec3, const float)>("PhysicsEngine.dll", "applyThrust");
	wrapPosition = DLLManager::getInstance().getFunction<void(*)(glm::vec3*, const glm::vec2, const glm::vec2)>("PhysicsEngine.dll", "wrapPosition");
	updatePhysics = DLLManager::getInstance().getFunction<void(*)(glm::vec3*, const float, const float)>("PhysicsEngine.dll", "updatePhysics");
}

PhysicsEngineManager::~PhysicsEngineManager() {}