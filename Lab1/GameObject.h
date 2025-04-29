#pragma once

#include <string>
#include "Transform.h"

struct GameObject {
    Transform* _transform = nullptr;
    std::string _meshTag;
    std::string _shaderTag;

    glm::vec3 forwardDirection = glm::vec3(0, 0, 1);  // Default forward (Z-axis)
    glm::vec3 velocity = glm::vec3(0);               // Initial velocity

    // Default constructor allows empty object creation
    GameObject() = default;

    // Constructor-based initialization
    GameObject(Transform* t, const std::string m, const std::string s)
        : _transform(t), _meshTag(m), _shaderTag(s) {}

    // Flexible init function (can be called manually)
    void init(Transform* t, const std::string m, const std::string s) {
        _transform = t;
        _meshTag = m;
        _shaderTag = s;
    }
};
