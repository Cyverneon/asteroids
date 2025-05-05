#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(std::shared_ptr<Transform> transform) :
	_transform(transform)
{
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
	PhysicsEngineManager::getInstance().applyThrust(&_velocity, _forwardDirection, thrust*delta);
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
	PhysicsEngineManager::getInstance().updatePhysics(&_velocity, _dragFactor, delta);
}

void PhysicsObject::wrapPosition(glm::vec2 boundaries, glm::vec2 offset)
{
	PhysicsEngineManager::getInstance().wrapPosition(&_transform->pos, boundaries, offset);
}

bool PhysicsObject::checkCollision(PhysicsObject* other)
{
	return PhysicsEngineManager::getInstance().checkCollisionRadius(_transform->pos, other->_transform->pos, _collisionRadius, other->_collisionRadius);
}

void PhysicsObject::updateForwardDirection()
{
	PhysicsEngineManager::getInstance().setForwardDirectionFromRot(&_forwardDirection, _transform->rot);
}