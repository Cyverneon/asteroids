#pragma once
#include "Transform.h"
#include "DLLManager.h"

struct PhysicsObject
{
public:
	PhysicsObject(std::shared_ptr<Transform> transform);
	~PhysicsObject();

	void updateRot(glm::vec3 newRot);
	void applyThrust(float thrust, float delta);
	void moveByVel(float delta);
	void moveByAmount(float amount, float delta);
	void updatePhysics(float delta);
	void wrapPosition(glm::vec2 boundaries, glm::vec2 offset);

	std::shared_ptr<Transform> _transform;

	glm::vec3 _forwardDirection = glm::vec3(0, 0, 1);
	glm::vec3 _velocity = glm::vec3(0);

	float _dragFactor = 2.0;

private:
	void updateForwardDirection();

	// Function pointers for physics engine functions
	void (*setForwardDirection_PE)(glm::vec3*, const glm::vec3) = nullptr;
	void (*setForwardDirectionFromRot_PE)(glm::vec3*, const glm::vec3) = nullptr;
	void (*applyThrust_PE)(glm::vec3*, const glm::vec3, const float) = nullptr;
	void (*wrapPosition_PE)(glm::vec3*, const glm::vec2, const glm::vec2) = nullptr;
	void (*updatePhysics_PE)(glm::vec3*, const float, const float) = nullptr;
};

