#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(std::shared_ptr<Transform> transform) :
	_transform(transform)
{
	if (!DLLManager::getInstance().loadDLL("PhysicsEngine.dll"))
	{
		std::cerr << "Failed to load PhysicsEngine.dll" << std::endl;
		return;
	}

	setForwardDirection_PE = DLLManager::getInstance().getFunction<void(*)(glm::vec3*, const glm::vec3)>("PhysicsEngine.dll", "setForwardDirection");
	setForwardDirectionFromRot_PE = DLLManager::getInstance().getFunction<void(*)(glm::vec3*, const glm::vec3)>("PhysicsEngine.dll", "setForwardDirectionFromRot");
	applyThrust_PE = DLLManager::getInstance().getFunction<void(*)(glm::vec3*, const glm::vec3, const float)>("PhysicsEngine.dll", "applyThrust");
	wrapPosition_PE = DLLManager::getInstance().getFunction<void(*)(glm::vec3*, const glm::vec2, const glm::vec2)>("PhysicsEngine.dll", "wrapPosition");
	updatePhysics_PE = DLLManager::getInstance().getFunction<void(*)(glm::vec3*, const float, const float)>("PhysicsEngine.dll", "updatePhysics");

	updateForwardDirection();
}

PhysicsObject::~PhysicsObject() {}


void PhysicsObject::updateRot(glm::vec3 newRot)
{
	_transform->rot = newRot;
	updateForwardDirection();
}

void PhysicsObject::applyThrust(float thrust, float delta)
{
	applyThrust_PE(&_velocity, _forwardDirection, thrust*delta);
}

void PhysicsObject::moveByVel(float delta)
{
	_transform->pos += _velocity * delta;
}

void PhysicsObject::moveByAmount(float amount, float delta)
{
	_transform->pos += glm::normalize(_forwardDirection) * amount * delta;
}

void PhysicsObject::updatePhysics(float delta)
{
	updatePhysics_PE(&_velocity, _dragFactor, delta);
}

void PhysicsObject::wrapPosition(glm::vec2 boundaries, glm::vec2 offset)
{
	wrapPosition_PE(&_transform->pos, boundaries, offset);
}

// private functions
void PhysicsObject::updateForwardDirection()
{
	setForwardDirectionFromRot_PE(&_forwardDirection, _transform->rot);
}