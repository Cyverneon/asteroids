#pragma once

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include "Transform.h"

class TransformManager {
public:
    static TransformManager& getInstance();  // Singleton instance

    std::shared_ptr<Transform> addTransform(const std::string& tag, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
    std::shared_ptr<Transform> getTransform(const std::string& tag);
    bool hasTransform(const std::string& tag);
    void removeTransform(const std::string& tag);

    void clear();  // Clears all stored transforms

private:
    TransformManager() = default;
    ~TransformManager() = default;
    TransformManager(const TransformManager&) = delete;
    TransformManager& operator=(const TransformManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<Transform>> _transforms;
};
