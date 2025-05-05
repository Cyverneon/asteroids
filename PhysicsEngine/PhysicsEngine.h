#pragma once

#ifdef PHYSICSENGINE_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

#include <glm/glm.hpp>

extern "C" PHYSICS_API void setForwardDirection(glm::vec3* forwardDirection, const glm::vec3 newForward);

extern "C" PHYSICS_API void setForwardDirectionFromRot(glm::vec3* forwardDirection, const glm::vec3 newRotation);

extern "C" PHYSICS_API void applyThrust(glm::vec3* vel, const glm::vec3 forwardDirection, const float thrustAmount);

extern "C" PHYSICS_API void wrapPosition(glm::vec3* pos, const glm::vec2 boundaries, const glm::vec2 offset);

extern "C" PHYSICS_API void updatePhysics(glm::vec3* vel, const float dragFactor, const float delta);

extern "C" PHYSICS_API bool checkCollisionRadius(glm::vec3 posA, glm::vec3 posB, float radiusA, float radiusB);

extern "C" PHYSICS_API bool checkCollisionAABB(glm::vec3 posA, glm::vec3 posB, glm::vec3 halfExtentsA, glm::vec3 halfExtentsB);