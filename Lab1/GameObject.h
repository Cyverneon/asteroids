#pragma once

#include <string>
#include "Transform.h"
#include "TransformManager.h"

struct GameObject {
    std::string _meshTag;
    std::string _shaderTag;

    glm::vec3 forwardDirection = glm::vec3(0, 0, 1);  // Default forward (Z-axis)
    glm::vec3 velocity = glm::vec3(0);               // Initial velocity

    // transforms should be unique to each game object anyway so they might as well always just be created and accessed from the GameObject class
    // the pointer is const (i.e., can't make a new transform and assign it to _transform), but the Transform it points to is not const and can be modified
    Transform* const _transform;

    // Constructor-based initialization
    GameObject(const std::string tag, const std::string meshTag, const std::string shaderTag)
        : _meshTag(meshTag),
        _shaderTag(shaderTag),
        _transform(TransformManager::getInstance().addTransform(tag, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)))
    {
    }
};
