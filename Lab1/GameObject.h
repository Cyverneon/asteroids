#pragma once

#include <vector>
#include <string>
#include "Transform.h"
#include "TransformManager.h"

struct GameObject {
    std::string _meshTag;
    std::string _shaderTag;
    std::vector<std::string> _textureTags;

    glm::vec3 forwardDirection = glm::vec3(0, 0, 1);  // Default forward (Z-axis)
    glm::vec3 velocity = glm::vec3(0);               // Initial velocity

    std::string _transformTag;
    // the pointer is const (i.e., can't make a new transform and assign it to _transform), but the Transform it points to is not const and can be modified
    std::shared_ptr<Transform> const _transform;

    // Constructor-based initialization
    GameObject(const std::string tag, const std::string meshTag, const std::string shaderTag, const std::vector<std::string> textureTags, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
        : _meshTag(meshTag),
        _shaderTag(shaderTag),
        _textureTags(textureTags),
        _transformTag(tag),
        // initialize _transform with the value returned from TransformManager creating a new transform
        _transform(TransformManager::getInstance().addTransform(tag, pos, rot, scale))
    {
    }

    ~GameObject()
    {
        // transform is removed as transforms are unique to each gameobject (i.e., nothing else should be using this transform)
        // meshes, shaders, textures can be shared so shouldn't be removed just because this gameobject using them was deleted
        TransformManager::getInstance().removeTransform(_transformTag);
    }
};
