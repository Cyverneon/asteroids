#include "PhysicsEngine.h"
#include <iostream>

extern "C" PHYSICS_API void setForwardDirection(glm::vec3* forwardDirection, const glm::vec3 newForward)
{
    *forwardDirection = glm::normalize(newForward);
}

extern "C" PHYSICS_API void setForwardDirectionFromRot(glm::vec3* forwardDirection, const glm::vec3 newRotation)
{
    // calculate new forward direction from 
    //forward.x = cos(pitch) * sin(yaw);
    //forward.y = -sin(pitch);
    //forward.z = cos(pitch) * cos(yaw);

    glm::vec3 newForward = glm::vec3(
        (cos(newRotation.x) * sin(newRotation.y)),
        -sin(newRotation.x),
        (cos(newRotation.x) * cos(newRotation.y)));

    *forwardDirection = glm::normalize(newForward);
}

extern "C" PHYSICS_API void applyThrust(glm::vec3* vel, const glm::vec3 forwardDirection, const float thrustAmount)
{
    *vel += glm::normalize(forwardDirection) * thrustAmount;
}

extern "C" PHYSICS_API void wrapPosition(glm::vec3* pos, const glm::vec2 boundaries, const glm::vec2 offset)
{
    if (pos->x > boundaries.x + offset.x)
        pos->x = -boundaries.x - offset.x;
    else if (pos->x < -boundaries.x - offset.x)
        pos->x = boundaries.x + offset.x;
    if (pos->z > boundaries.y + offset.y)
        pos->z = -boundaries.y - offset.y;
    else if (pos->z < -boundaries.y - offset.y)
        pos->z = boundaries.y + offset.y;
}

extern "C" PHYSICS_API void updatePhysics(glm::vec3* vel, const float dragFactor, const float delta)
{
    *vel -= *vel * dragFactor * delta;
}

extern "C" PHYSICS_API bool checkCollisionRadius(const PhysicsObject* a, const PhysicsObject* b, float radiusA, float radiusB)
{
    float distance = glm::distance(a->_transform->pos, b->_transform->pos);
    return distance < (radiusA + radiusB);
}

extern "C" PHYSICS_API bool checkCollisionAABB(const PhysicsObject* a, const PhysicsObject* b, const glm::vec3& halfExtentsA, const glm::vec3& halfExtentsB)
{
    return (
        abs(a->_transform->pos.x - b->_transform->pos.x) < (halfExtentsA.x + halfExtentsB.x) &&
        abs(a->_transform->pos.y - b->_transform->pos.y) < (halfExtentsA.y + halfExtentsB.y) &&
        abs(a->_transform->pos.z - b->_transform->pos.z) < (halfExtentsA.z + halfExtentsB.z)
        );
}