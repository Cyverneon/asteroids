#pragma once

#include <string>
#include "Transform.h"
#include "TransformManager.h"

struct GameObject {
    std::string _meshTag;
    std::string _shaderTag;

    glm::vec3 forwardDirection = glm::vec3(0, 0, 1);  // Default forward (Z-axis)
    glm::vec3 velocity = glm::vec3(0);               // Initial velocity

    // the pointer is const (i.e., can't make a new transform and assign it to _transform), but the Transform it points to is not const and can be modified
    Transform* const _transform;

    // Constructor-based initialization
    GameObject(const std::string tag, const std::string meshTag, const std::string shaderTag, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
        : _meshTag(meshTag),
        _shaderTag(shaderTag),
        // initialize _transform with the value returned from TransformManager creating a new transform
        _transform(TransformManager::getInstance().addTransform(tag, pos, rot, scale))
    {
    }
};
