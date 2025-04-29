#include "PhysicsEngine.h"
#include <iostream>

extern "C" PHYSICS_API void setForwardDirection(GameObject* obj, glm::vec3 newForward)
{
    obj->forwardDirection = glm::normalize(newForward);
}

extern "C" PHYSICS_API void applyThrust(GameObject* obj, float thrustAmount)
{
    obj->velocity += obj->forwardDirection * thrustAmount;
}

extern "C" PHYSICS_API void updatePhysics(GameObject * obj, float deltaTime)
{
    float dragFactor = 2.0f;
    obj->velocity -= (obj->velocity*dragFactor)*deltaTime;
}

extern "C" PHYSICS_API bool checkCollisionRadius(const GameObject* a, const GameObject* b, float radiusA, float radiusB)
{
    float distance = glm::distance(a->_transform->pos, b->_transform->pos);
    return distance < (radiusA + radiusB);
}

extern "C" PHYSICS_API bool checkCollisionAABB(const GameObject* a, const GameObject* b, const glm::vec3& halfExtentsA, const glm::vec3& halfExtentsB)
{
    return (
        abs(a->_transform->pos.x - b->_transform->pos.x) < (halfExtentsA.x + halfExtentsB.x) &&
        abs(a->_transform->pos.y - b->_transform->pos.y) < (halfExtentsA.y + halfExtentsB.y) &&
        abs(a->_transform->pos.z - b->_transform->pos.z) < (halfExtentsA.z + halfExtentsB.z)
        );
}