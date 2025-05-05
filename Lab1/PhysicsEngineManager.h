#pragma once

#include <glm/glm.hpp>
#include "DLLManager.h"

class PhysicsEngineManager
{
public:
	static PhysicsEngineManager& getInstance();

	// Function pointers for physics engine functions
	void (*setForwardDirection)(glm::vec3*, const glm::vec3) = nullptr;
	void (*setForwardDirectionFromRot)(glm::vec3*, const glm::vec3) = nullptr;
	void (*applyThrust)(glm::vec3*, const glm::vec3, const float) = nullptr;
	void (*wrapPosition)(glm::vec3*, const glm::vec2, const glm::vec2) = nullptr;
	void (*updatePhysics)(glm::vec3*, const float, const float) = nullptr;

private:
	PhysicsEngineManager();
	~PhysicsEngineManager();
};

