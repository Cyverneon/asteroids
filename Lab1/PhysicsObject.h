#pragma once
#include "Transform.h"
#include "PhysicsEngineManager.h"

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
};

