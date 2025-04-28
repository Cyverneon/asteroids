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
    float distance = glm::distance(a->transform->pos, b->transform->pos);
    return distance < (radiusA + radiusB);
}

extern "C" PHYSICS_API bool checkCollisionAABB(const GameObject* a, const GameObject* b, const glm::vec3& halfExtentsA, const glm::vec3& halfExtentsB)
{
    return (
        abs(a->transform->pos.x - b->transform->pos.x) < (halfExtentsA.x + halfExtentsB.x) &&
        abs(a->transform->pos.y - b->transform->pos.y) < (halfExtentsA.y + halfExtentsB.y) &&
        abs(a->transform->pos.z - b->transform->pos.z) < (halfExtentsA.z + halfExtentsB.z)
        );
}